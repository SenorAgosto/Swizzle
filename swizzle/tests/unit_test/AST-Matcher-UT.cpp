#include "./platform/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Matcher.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
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

    struct HasChildOfFixture : public MatcherFixture
    {
        const Token token = Token("// comment", 0, 10, TokenType::comment);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(HasChildOfFixture, verifyHasChildOf)
    {
        Matcher m = Matcher().hasChildOf<nodes::Comment>();
        CHECK(!m(ast));

        detail::appendNode<nodes::Comment>(nodeStack, info);
        CHECK(m(ast));
    }

    struct HasChildNotOfFixture : public HasChildOfFixture
    {
    };

    TEST_FIXTURE(HasChildNotOfFixture, verifyHasChildNotOf)
    {
        Matcher m = Matcher().hasChildNotOf<nodes::Comment, nodes::MultilineComment>();
        CHECK(!m(ast));

        detail::appendNode<nodes::Comment>(nodeStack, info);
        CHECK(!m(ast));

        detail::appendNode<nodes::MultilineComment>(nodeStack, info);
        CHECK(!m(ast));

        detail::appendNode<Node>(nodeStack);
        CHECK(m(ast));
    }
}
