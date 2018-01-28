#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/nodes/VariableBlockCase.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/StructVariableBlockNamespaceFirstColonReadState.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;
    using namespace swizzle::types;

    struct StructVariableBlockNamespaceFirstColonReadStateFixture
    {
        StructVariableBlockNamespaceFirstColonReadStateFixture()
        {
            nodeStack.push(ast.root());
        }

        states::StructVariableBlockNamespaceFirstColonReadState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        NodeStack attributeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(StructVariableBlockNamespaceFirstColonReadStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsColon : public StructVariableBlockNamespaceFirstColonReadStateFixture
    {
        const TokenInfo token = TokenInfo(Token(":", 0, 1, TokenType::colon), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsColon, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(token, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructVariableBlockNamespaceSecondColonRead, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsInvalid : public StructVariableBlockNamespaceFirstColonReadStateFixture
    {
        const TokenInfo token = TokenInfo(Token(";", 0, 1, TokenType::end_statement), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(token, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }
}
