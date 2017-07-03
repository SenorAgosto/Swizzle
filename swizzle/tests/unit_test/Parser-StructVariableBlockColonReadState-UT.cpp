#include "./ut_support/UnitTestSupport.hpp"
#include <swizzle/parser/states/StructVariableBlockColonReadState.hpp>

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/StructVectorState.hpp>

namespace{

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct StructVariableBlockColonReadFixture
    {
        StructVariableBlockColonReadFixture()
        {
            nodeStack.push(ast.root());
        }

        states::StructVariableBlockColonReadState state;
        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(StructVariableBlockColonReadFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsString : public StructVariableBlockColonReadFixture
    {
        const TokenInfo token = TokenInfo(Token("field1", 0, 6, TokenType::string), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsString, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(token, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructVariableBlockOnField, parserState);
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(1U, tokenStack.size());\
    }

    struct WhenNextTokenIsNotString : public StructVariableBlockColonReadFixture
    {
        const TokenInfo token = TokenInfo(Token(";", 0, 1, TokenType::end_statement), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsNotString, verifyConsume)
    {
        CHECK_THROW(state.consume(token, nodeStack, tokenStack, context), swizzle::SyntaxError);
    }
}
