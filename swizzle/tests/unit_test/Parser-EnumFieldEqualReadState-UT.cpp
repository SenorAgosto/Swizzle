#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/Matcher.hpp>
#include <swizzle/ast/nodes/Enum.hpp>
#include <swizzle/ast/nodes/EnumField.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/EnumFieldEqualReadState.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct EnumFieldEqualReadStateFixture
    {
        EnumFieldEqualReadStateFixture()
        {
            nodeStack.push(ast.root());

            const auto enumInfo = TokenInfo(Token("enum", 0, 3, TokenType::keyword), FileInfo("test.swizzle"));
            const auto enumName = TokenInfo(Token("my_enum", 0, 7, TokenType::string), FileInfo("test.swizzle"));

            auto node = swizzle::parser::detail::appendNode<nodes::Enum>(nodeStack, enumInfo, enumName, "my_namespace");
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
        NodeStack attributeStack;
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::EnumFieldValueRead, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        const auto& field = dynamic_cast<nodes::EnumField&>(*nodeStack.top());
        const auto& value = field.value();

        REQUIRE CHECK_EQUAL(0, value.which());
        CHECK_EQUAL(1U, boost::get<std::uint8_t>(value));
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::EnumFieldValueRead, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        const auto& field = dynamic_cast<nodes::EnumField&>(*nodeStack.top());
        const auto& value = field.value();

        REQUIRE CHECK_EQUAL(0, value.which());
        CHECK_EQUAL(44U, boost::get<std::uint8_t>(value));
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::EnumFieldValueRead, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        const auto& field = dynamic_cast<nodes::EnumField&>(*nodeStack.top());
        const auto& value = field.value();

        REQUIRE CHECK_EQUAL(0, value.which());
        CHECK_EQUAL(97U, boost::get<std::uint8_t>(value));
    }

    struct WhenNextTokenIsInvalid : public EnumFieldEqualReadStateFixture
    {
        const Token token = Token(";", 0, 1, TokenType::end_statement);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
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
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::ParserError);
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
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::ParserError);
    }
}
