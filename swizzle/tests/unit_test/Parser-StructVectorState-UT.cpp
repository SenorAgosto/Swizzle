#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Matcher.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Attribute.hpp>
#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/nodes/StructField.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/StructVectorState.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct StructVectorStateFixture
    {
        StructVectorStateFixture()
        {
            nodeStack.push(ast.root());

            auto node = detail::appendNode<nodes::Struct>(nodeStack, structKeyword, name, "my_namespace");

            context.TypeCache["my_namespace::MyStruct"] = node;
            nodeStack.push(node);
        }

        states::StructVectorState state;

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

    TEST_FIXTURE(StructVectorStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsDot : public StructVectorStateFixture
    {
        const Token token = Token(".", 0, 1, TokenType::dot);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsDot, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructVectorNestedOnMember, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsRightBracket : public StructVectorStateFixture
    {
        WhenNextTokenIsRightBracket()
        {
            context.CurrentNamespace = "my_namespace";

            auto node = detail::appendNode<nodes::StructField>(nodeStack);
            auto& field = static_cast<nodes::StructField&>(*node);
            field.name(fieldName);
            field.type("u8");

            nodeStack.push(node);
            tokenStack.push(name);
        }

        const Token field1 = Token("field1", 0, 6, TokenType::string);
        const Token fieldNameToken = Token("field1", 0, 6, TokenType::string);
        const Token token = Token("]", 0, 1, TokenType::r_bracket);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
        const TokenInfo name = TokenInfo(field1, fileInfo);
        const TokenInfo fieldName = TokenInfo(fieldNameToken, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsRightBracket, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(1U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        auto matcher = Matcher().getChildrenOf<nodes::StructField>().bind("field");
        REQUIRE CHECK(matcher(nodeStack.top()));

        const auto fieldNode = matcher.bound("field_0");
        REQUIRE CHECK(fieldNode);

        const auto& field = static_cast<nodes::StructField&>(*fieldNode);
        CHECK(field.isVector());
        CHECK_EQUAL("field1", field.vectorSizeMember().token().value());
    }

    struct WhenNextTokenIsInvalid : public StructVectorStateFixture
    {
        const Token token = Token(";", 0, 1, TokenType::end_statement);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }
}
