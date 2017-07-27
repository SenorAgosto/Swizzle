#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Attribute.hpp>
#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/nodes/StructField.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/StructStartArrayState.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct StructStartArrayStateFixture
    {
        StructStartArrayStateFixture()
        {
            nodeStack.push(ast.root());

            const auto structKeyword = TokenInfo(Token("struct", 0, 6, TokenType::keyword), FileInfo("test.swizzle"));
            const auto name = TokenInfo(Token("MyStruct", 0, 8, TokenType::string), FileInfo("test.swizzle"));

            auto node = detail::appendNode<nodes::Struct>(nodeStack, structKeyword, name, "my_namespace");
            nodeStack.push(node);

            node = detail::appendNode<nodes::StructField>(nodeStack);
            nodeStack.push(node);
        }

        states::StructStartArrayState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        NodeStack attributeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(StructStartArrayStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsNumericLiteral : public StructStartArrayStateFixture
    {
        const Token token = Token("10", 0, 2, TokenType::numeric_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsNumericLiteral, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructArray, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        REQUIRE CHECK(detail::nodeStackTopIs<nodes::StructField>(nodeStack));
        const auto& field = static_cast<nodes::StructField&>(*nodeStack.top());

        CHECK(field.isArray());
        CHECK_EQUAL(10, field.arraySize());
    }

    struct WhenNextTokenIsNegativeNumericLiteral : public StructStartArrayStateFixture
    {
        const Token token = Token("-10", 0, 3, TokenType::numeric_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsNegativeNumericLiteral, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }

    struct WhenNextTokenIsString : public StructStartArrayStateFixture
    {
        const Token token = Token("boyah", 0, 5, TokenType::string);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsString, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructVector, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());
    }

    struct WhenNextTokenIsInvalid : public StructStartArrayStateFixture
    {
        const Token token = Token(":", 0, 1, TokenType::colon);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }
}
