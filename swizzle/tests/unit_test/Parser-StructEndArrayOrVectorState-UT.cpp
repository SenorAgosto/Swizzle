#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/StructEndArrayOrVectorState.hpp>

#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/nodes/StructField.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct StructEndArrayOrVectorStateFixture
    {
        StructEndArrayOrVectorStateFixture()
        {
            nodeStack.push(ast.root());

            auto node = detail::appendNode<nodes::Struct>(nodeStack, structKeyword, name, "my_namespace");

            context.TypeCache["my_namespace::MyStruct"] = node;
            nodeStack.push(node);

            node = detail::appendNode<nodes::StructField>(nodeStack);
            nodeStack.push(node);
        }

        states::StructEndArrayOrVectorState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        NodeStack attributeStack;
        TokenStack tokenStack;
        ParserStateContext context;

        const FileInfo fileInfo = FileInfo("test.swizzle");
        const Token s = Token("struct", 0, 6, TokenType::keyword);
        const Token n = Token("MyStruct", 0, 8, TokenType::string);

        const TokenInfo structKeyword = TokenInfo(s, fileInfo);
        const TokenInfo name = TokenInfo(n, fileInfo);
    };

    TEST_FIXTURE(StructEndArrayOrVectorStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsString : public StructEndArrayOrVectorStateFixture
    {
        const Token token = Token("field1", 0, 6, TokenType::string);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsString, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        REQUIRE CHECK(detail::nodeStackTopIs<nodes::StructField>(nodeStack));
        const auto& top = static_cast<nodes::StructField&>(*nodeStack.top());

        CHECK_EQUAL("field1", top.name().token().to_string());
    }

    struct WhenNextTokenIsNotString : public StructEndArrayOrVectorStateFixture
    {
        const Token token = Token(";", 0, 1, TokenType::end_statement);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsNotString, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }
}
