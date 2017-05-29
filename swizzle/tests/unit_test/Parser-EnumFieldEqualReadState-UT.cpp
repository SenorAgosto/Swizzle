#include "./platform/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractTreeEdgeVisitorInterface.hpp>
#include <swizzle/ast/AbstractTreeVertexVisitorInterface.hpp>
#include <swizzle/ast/AbstractTreeVisitorInterface.hpp>

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Enum.hpp>
#include <swizzle/ast/nodes/EnumField.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/EnumFieldEqualReadState.hpp>

#include <swizzle/Exceptions.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct EnumFieldEqualReadStateFixture
    {
        EnumFieldEqualReadStateFixture()
        {
            nodeStack.push(ast.root());

            auto node = swizzle::parser::detail::appendNode<nodes::Enum>(nodeStack, TokenInfo(Token("my_enum", 0, 7, TokenType::string), FileInfo("test.swizzle")), "my_namespace");
            auto& enumNode = static_cast<nodes::Enum&>(*node);
            enumNode.underlying(TokenInfo(Token("u8", 0, 2, TokenType::type), FileInfo("test.swizzle")));
            nodeStack.push(node);

            const TokenInfo info(Token("field1", 0, 6, TokenType::string), FileInfo("test.swizzle"));
            const TokenInfo underlying(Token("u8", 0, 2, TokenType::type), FileInfo("test.swizzle"));

            node = swizzle::parser::detail::appendNode<nodes::EnumField>(nodeStack, info, underlying);
            nodeStack.push(node);
        }

        states::EnumFieldEqualReadState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(EnumFieldEqualReadStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsHexLiteral : public EnumFieldEqualReadStateFixture
    {
        const Token token = Token("0x01", 0, 4, TokenType::hex_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsHexLiteral, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::EnumFieldValueRead, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        // TODO: check the enum field value
    }

    struct WhenNextTokenIsNumericLiteral : public EnumFieldEqualReadStateFixture
    {
        const Token token = Token("44", 0, 2, TokenType::numeric_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsNumericLiteral, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::EnumFieldValueRead, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        // TODO: check the enum field value
    }

    struct WhenNextTokenIsCharLiteral : public EnumFieldEqualReadStateFixture
    {
        const Token token = Token("'a'", 0, 3, TokenType::char_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsCharLiteral, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::EnumFieldValueRead, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        // TODO: check the enum field value
    }

    struct WhenNextTokenIsInvalid : public EnumFieldEqualReadStateFixture
    {
        const Token token = Token(";", 0, 1, TokenType::end_statement);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::SyntaxError);
    }

    struct WhenNextTokenIsHexLiteralButTopOfStackIsNotEnumField : public WhenNextTokenIsHexLiteral
    {
        WhenNextTokenIsHexLiteralButTopOfStackIsNotEnumField()
        {
            nodeStack.pop();
        }
    };

    TEST_FIXTURE(WhenNextTokenIsHexLiteralButTopOfStackIsNotEnumField, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::ParserError);
    }

    struct WhenNextTokenIsHexLiteralButSecondEntryOnStackIsNotEnum : public WhenNextTokenIsHexLiteral
    {
        WhenNextTokenIsHexLiteralButSecondEntryOnStackIsNotEnum()
        {
            const auto node = nodeStack.top();
            nodeStack.pop();
            nodeStack.pop();
            nodeStack.push(node);
        }
    };

    TEST_FIXTURE(WhenNextTokenIsHexLiteralButSecondEntryOnStackIsNotEnum, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::ParserError);
    }
}
