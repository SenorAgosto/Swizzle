#include "./platform/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Matcher.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::parser;

    struct MatcherFixture
    {
        MatcherFixture()
        {
            nodeStack.emplace(ast.root());
        }

        AbstractSyntaxTree ast;
        NodeStack nodeStack;
    };

    TEST_FIXTURE(MatcherFixture, verifyConstruction)
    {
        Matcher m;
    }

    TEST_FIXTURE(MatcherFixture, verifyHasChild)
    {
        Matcher m = Matcher().hasChild();

        CHECK(!m(ast));

        detail::appendNode<Node>(nodeStack);
        CHECK(m(ast));
    }
}
