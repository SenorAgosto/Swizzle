#include "./platform/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractTreeEdgeVisitorInterface.hpp>
#include <swizzle/ast/AbstractTreeVertexVisitorInterface.hpp>
#include <swizzle/ast/AbstractTreeVisitorInterface.hpp>

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/Import.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/TranslationUnitMainState.hpp>

#include <swizzle/Exceptions.hpp>

#include <array>
#include <boost/filesystem.hpp>
#include <iostream>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct TranslationUnitMainStateFixture
    {
        TranslationUnitMainStateFixture()
        {
            nodeStack.push(ast.root());
            context.CurrentEnumValue = 100; // something non-zero so we can ensure this is reset
        }

        states::TranslationUnitMainState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(TranslationUnitMainStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsComment : public TranslationUnitMainStateFixture
    {
        const Token token = Token("// comment", 0, 10, TokenType::comment);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsComment, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        // TODO: validate a comment was added to the AST
    }

    struct WhenNextTokenIsMultilineComment : public TranslationUnitMainStateFixture
    {
        const Token token = Token("// comment \\\n\t comment2", 0, 23, TokenType::comment);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsMultilineComment, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        // TODO: validate a comment was added to the AST
    }

    struct WhenNextTokenIsUsingKeyword : public TranslationUnitMainStateFixture
    {
        const Token token = Token("using", 0, 5, TokenType::keyword);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsUsingKeyword, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StartUsing, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());
    }

    struct WhenNextTokenIsEnumKeyword : public TranslationUnitMainStateFixture
    {
        const Token token = Token("enum", 0, 5, TokenType::keyword);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsEnumKeyword, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());
        CHECK_EQUAL(100U, context.CurrentEnumValue);

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StartEnum, parserState);
        CHECK_EQUAL(0U, context.CurrentEnumValue);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());
    }

    struct WhenNextTokenIsBitfieldKeyword : public TranslationUnitMainStateFixture
    {
        const Token token = Token("bitfield", 0, 8, TokenType::keyword);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsBitfieldKeyword, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StartBitfield, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsStructKeyword : public TranslationUnitMainStateFixture
    {
        const Token token = Token("struct", 0, 6, TokenType::keyword);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsStructKeyword, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StartStruct, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsAttribute : public TranslationUnitMainStateFixture
    {
        const Token token = Token("@inbound", 0, 8, TokenType::attribute);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsAttribute, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        // TODO: validate a comment was added to the AST
    }

    struct WhenNextTokenIsAttributeBlock : public TranslationUnitMainStateFixture
    {
        std::array<TokenInfo, 2> tokens = {{
              TokenInfo(Token("@validate", 0, 9, TokenType::attribute), FileInfo("test.swizzle"))
            , TokenInfo(Token("{size != 0}", 0, 11, TokenType::attribute_block), FileInfo("test.swizzle"))
        }};
    };

    TEST_FIXTURE(WhenNextTokenIsAttributeBlock, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        auto parserState = state.consume(tokens[0], nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        parserState = state.consume(tokens[1], nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        // TODO: validate a comment was added to the AST
    }

    struct WhenNextTokenIsHexLiteralAfterAttribute : public TranslationUnitMainStateFixture
    {
        std::array<TokenInfo, 2> tokens = {{
              TokenInfo(Token("@validate", 0, 9, TokenType::attribute), FileInfo("test.swizzle"))
            , TokenInfo(Token("0x01", 0, 4, TokenType::hex_literal), FileInfo("test.swizzle"))
        }};
    };

    TEST_FIXTURE(WhenNextTokenIsHexLiteralAfterAttribute, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        auto parserState = state.consume(tokens[0], nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        parserState = state.consume(tokens[1], nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        // TODO: check the AST is correct
    }

    struct WhenNextTokenIsCharLiteralAfterAttribute : public TranslationUnitMainStateFixture
    {
        std::array<TokenInfo, 2> tokens = {{
              TokenInfo(Token("@validate", 0, 9, TokenType::attribute), FileInfo("test.swizzle"))
            , TokenInfo(Token("'a'", 0, 3, TokenType::char_literal), FileInfo("test.swizzle"))
        }};
    };

    TEST_FIXTURE(WhenNextTokenIsCharLiteralAfterAttribute, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        auto parserState = state.consume(tokens[0], nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        parserState = state.consume(tokens[1], nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        // TODO: check the AST is correct
    }

    struct WhenNextTokenIsStringLiteralAfterAttribute : public TranslationUnitMainStateFixture
    {
        std::array<TokenInfo, 2> tokens = {{
              TokenInfo(Token("@validate", 0, 9, TokenType::attribute), FileInfo("test.swizzle"))
            , TokenInfo(Token("\"string\"", 0, 3, TokenType::string_literal), FileInfo("test.swizzle"))
        }};
    };

    TEST_FIXTURE(WhenNextTokenIsStringLiteralAfterAttribute, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        auto parserState = state.consume(tokens[0], nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        parserState = state.consume(tokens[1], nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        // TODO: check the AST is correct
    }

    struct WhenNextTokenIsCommentAfterAnAttribute : public TranslationUnitMainStateFixture
    {
        std::array<TokenInfo, 2> tokens = {{
              TokenInfo(Token("@validate", 0, 9, TokenType::attribute), FileInfo("test.swizzle"))
            , TokenInfo(Token("// comment", 0, 10, TokenType::comment), FileInfo("test.swizzle"))
        }};
    };

    TEST_FIXTURE(WhenNextTokenIsCommentAfterAnAttribute, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        auto parserState = state.consume(tokens[0], nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        parserState = state.consume(tokens[1], nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }
}
