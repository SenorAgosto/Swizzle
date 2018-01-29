#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/ast/nodes/VariableBlock.hpp>
#include <swizzle/ast/nodes/VariableBlockCase.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/StructVariableBlockCaseValueColonReadState.hpp>
#include <swizzle/types/utils/AppendNode.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;
    using namespace swizzle::types;
    
    struct StructVariableBlockCaseValueColonReadStateFixture
    {
        StructVariableBlockCaseValueColonReadStateFixture()
        {
            nodeStack.push(ast.root());
        }

        states::StructVariableBlockCaseValueColonReadState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        NodeStack attributeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(StructVariableBlockCaseValueColonReadStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsString : public StructVariableBlockCaseValueColonReadStateFixture
    {
        const TokenInfo token = TokenInfo(Token("MyStruct", 0, 8, TokenType::string), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsString, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(token, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructVariableBlockCaseBlockNameRead, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());

        CHECK_EQUAL("MyStruct", tokenStack.top().token().value());
    }

    struct WhenNextTokenIsNotString : public StructVariableBlockCaseValueColonReadStateFixture
    {
        const TokenInfo token = TokenInfo(Token(";", 0, 1, TokenType::end_statement), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsNotString, verifyConsume)
    {
        CHECK_THROW(state.consume(token, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }
}
