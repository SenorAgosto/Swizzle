#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Attribute.hpp>
#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/nodes/StructField.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/StructArrayState.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct StructArrayStateFixture
    {
        StructArrayStateFixture()
        {
            nodeStack.push(ast.root());

            const auto structKeyword = TokenInfo(Token("struct", 0, 6, TokenType::keyword), FileInfo("test.swizzle"));
            const auto name = TokenInfo(Token("MyStruct", 0, 8, TokenType::string), FileInfo("test.swizzle"));

            auto node = detail::appendNode<nodes::Struct>(nodeStack, structKeyword, name, "my_namespace");
            nodeStack.push(node);

            node = detail::appendNode<nodes::StructField>(nodeStack);
            nodeStack.push(node);
        }

        states::StructArrayState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(StructArrayStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsRightBracket : public StructArrayStateFixture
    {
        const Token token = Token("]", 0, 1, TokenType::r_bracket);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsRightBracket, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructStartScope, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsInvalid : public StructArrayStateFixture
    {
        const Token token = Token(":", 0, 1, TokenType::colon);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::SyntaxError);
    }
}
