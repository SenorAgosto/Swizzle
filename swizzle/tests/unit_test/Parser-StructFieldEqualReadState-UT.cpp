#include "./ut_support/UnitTestSupport.hpp"
#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/StructFieldEqualReadState.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct StructFieldEqualReadStateFixture
    {
        StructFieldEqualReadStateFixture()
        {
            nodeStack.push(ast.root());
        }

        states::StructFieldEqualReadState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        NodeStack attributeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(StructFieldEqualReadStateFixture, verifyConstruction)
    {
    }

    //struct WhenNextTokenIsNumericLiteral :
}
