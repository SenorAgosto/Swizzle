#include "./ut_support/UnitTestSupport.hpp"
#include <swizzle/parser/states/StructStartVariableBlockState.hpp>

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/StructVectorState.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct StructStartVariableBlockFixture
    {
        StructStartVariableBlockFixture()
        {
            nodeStack.push(ast.root());
        }

        states::StructStartVariableBlockState state;
        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        NodeStack attributeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(StructStartVariableBlockFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsColon : public StructStartVariableBlockFixture
    {
        const TokenInfo token = TokenInfo(Token(":", 0, 1, TokenType::colon), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsColon, verifyConsume)
    {
        const auto parserState = state.consume(token, nodeStack, attributeStack, tokenStack, context);
        CHECK_EQUAL(ParserState::StructVariableBlockColonRead, parserState);
    }

    struct WhenNextTokenIsNotColon : public StructStartVariableBlockFixture
    {
        const TokenInfo token = TokenInfo(Token(";", 0, 1, TokenType::end_statement), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsNotColon, verifyConsume)
    {
        CHECK_THROW(state.consume(token, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }
}
