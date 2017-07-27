#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/nodes/VariableBlockCase.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/StructVariableBlockNamespaceSecondColonReadState.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct StructVariableBlockNamespaceSecondColonReadStateFixture
    {
        StructVariableBlockNamespaceSecondColonReadStateFixture()
        {
            nodeStack.push(ast.root());
        }

        states::StructVariableBlockNamespaceSecondColonReadState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        NodeStack attributeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(StructVariableBlockNamespaceSecondColonReadStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsString : public StructVariableBlockNamespaceSecondColonReadStateFixture
    {
        const TokenInfo token = TokenInfo(Token("MyStruct", 0, 8, TokenType::string), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsString, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(token, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructVariableBlockCaseBlockNameRead, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());
    }

    struct WhenNextTokenIsInvalid : public StructVariableBlockNamespaceSecondColonReadStateFixture
    {
        const TokenInfo token = TokenInfo(Token(";", 0, 1, TokenType::end_statement), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(token, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }
}
