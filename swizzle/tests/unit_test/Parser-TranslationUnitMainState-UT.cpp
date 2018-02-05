#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Matcher.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Attribute.hpp>
#include <swizzle/ast/nodes/AttributeBlock.hpp>
#include <swizzle/ast/nodes/CharLiteral.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/HexLiteral.hpp>
#include <swizzle/ast/nodes/Import.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/ast/nodes/StringLiteral.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/TranslationUnitMainState.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>

#include <array>
#include <cstdint>
#include <memory>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;
    using namespace swizzle::types;

    struct TranslationUnitMainStateFixture
    {
        TranslationUnitMainStateFixture()
        {
            nodeStack.push(ast.root());
        }

        states::TranslationUnitMainState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        NodeStack attributeStack;
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
        auto matcher = Matcher().hasChildOf<nodes::Comment>();

        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());
        CHECK(!matcher(nodeStack.top()));

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
        CHECK(matcher(nodeStack.top()));
    }

    struct WhenNextTokenIsMultilineComment : public TranslationUnitMainStateFixture
    {
        const Token token = Token("// comment \\\n\t comment2", 0, 23, TokenType::multiline_comment);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsMultilineComment, verifyConsume)
    {
        auto matcher = Matcher().hasChildOf<nodes::MultilineComment>();

        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());
        CHECK(!matcher(nodeStack.top()));

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
        CHECK(matcher(nodeStack.top()));
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StartUsing, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StartEnum, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());
    }

    struct WhenNextTokenIsBitfieldKeyword : public TranslationUnitMainStateFixture
    {
        const Token token = Token("bitfield", 0, 8, TokenType::type);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsBitfieldKeyword, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StartBitfield, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StartStruct, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
        CHECK(utils::nodeStackTopIs<nodes::Attribute>(attributeStack));
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        auto parserState = state.consume(tokens[0], nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        parserState = state.consume(tokens[1], nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        auto matcher = Matcher().hasChildOf<nodes::AttributeBlock>();
        REQUIRE CHECK(utils::nodeStackTopIs<nodes::Attribute>(attributeStack));
        REQUIRE CHECK(matcher(attributeStack.top()));
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        auto parserState = state.consume(tokens[0], nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        parserState = state.consume(tokens[1], nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        auto matcher = Matcher().getChildrenOf<nodes::HexLiteral>().bind("hex");
        REQUIRE CHECK(utils::nodeStackTopIs<nodes::Attribute>(attributeStack));
        REQUIRE CHECK(matcher(attributeStack.top()));

        const auto hexLiteralNode = matcher.bound("hex_0");
        REQUIRE CHECK(hexLiteralNode);

        const auto& hex = static_cast<nodes::HexLiteral&>(*hexLiteralNode);
        CHECK_EQUAL("0x01", hex.info().token().value());
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        auto parserState = state.consume(tokens[0], nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        parserState = state.consume(tokens[1], nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        auto matcher = Matcher().getChildrenOf<nodes::CharLiteral>().bind("char");

        REQUIRE CHECK(utils::nodeStackTopIs<nodes::Attribute>(attributeStack));
        REQUIRE CHECK(matcher(attributeStack.top()));

        const auto charLiteralNode = matcher.bound("char_0");
        REQUIRE CHECK(charLiteralNode);

        const auto& charValue = static_cast<nodes::CharLiteral&>(*charLiteralNode);
        CHECK_EQUAL("'a'", charValue.info().token().value());
    }

    struct WhenNextTokenIsStringLiteralAfterAttribute : public TranslationUnitMainStateFixture
    {
        std::array<TokenInfo, 2> tokens = {{
              TokenInfo(Token("@validate", 0, 9, TokenType::attribute), FileInfo("test.swizzle"))
            , TokenInfo(Token("\"string\"", 0, 8, TokenType::string_literal), FileInfo("test.swizzle"))
        }};
    };

    TEST_FIXTURE(WhenNextTokenIsStringLiteralAfterAttribute, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        auto parserState = state.consume(tokens[0], nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        parserState = state.consume(tokens[1], nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        auto matcher = Matcher().getChildrenOf<nodes::StringLiteral>().bind("string");
        REQUIRE CHECK(utils::nodeStackTopIs<nodes::Attribute>(attributeStack));
        REQUIRE CHECK(matcher(attributeStack.top()));

        const auto stringLiteralNode = matcher.bound("string_0");
        REQUIRE CHECK(stringLiteralNode);

        const auto& s = static_cast<nodes::StringLiteral&>(*stringLiteralNode);
        CHECK_EQUAL("\"string\"", s.info().token().value());
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        auto parserState = state.consume(tokens[0], nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        parserState = state.consume(tokens[1], nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }
}
