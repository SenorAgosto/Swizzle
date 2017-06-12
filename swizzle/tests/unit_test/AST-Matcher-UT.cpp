#include "./platform/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Matcher.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Bitfield.hpp>
#include <swizzle/ast/nodes/BitfieldField.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/nodes/StructField.hpp>
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

    template<class Matcher>
    class TestInterface
    {
    public:
        TestInterface(Matcher& matcher)
            : matcher_(matcher)
        {
        }

        Matcher& testInterface()
        {
            return matcher_;
        }

    private:
        Matcher& matcher_;
    };

    TEST_FIXTURE(MatcherFixture, verifyExtensibility)
    {
        using TestMatcher = ExtensibleMatcher<TestInterface>;
        auto t = TestMatcher().testInterface();
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

    struct StructHasMemberNamedFixture : public MatcherFixture
    {
        StructHasMemberNamedFixture()
        {
            auto node = detail::appendNode<nodes::Struct>(nodeStack, info, info2, "my_namespace");
            nodeStack.push(node);

            node = detail::appendNode<nodes::StructField>(nodeStack);
            auto& field1 = static_cast<nodes::StructField&>(*node);
            field1.name(TokenInfo(Token("field1", 0, 6, TokenType::string), FileInfo("test.swizzle")));

            node = detail::appendNode<nodes::StructField>(nodeStack);
            auto& field2 = static_cast<nodes::StructField&>(*node);
            field2.name(TokenInfo(Token("field2", 0, 6, TokenType::string), FileInfo("test.swizzle")));

            node = detail::appendNode<nodes::StructField>(nodeStack);
            auto& field3 = static_cast<nodes::StructField&>(*node);
            field3.name(TokenInfo(Token("field3", 0, 6, TokenType::string), FileInfo("test.swizzle")));
        }

        const Token token = Token("struct", 0, 6, TokenType::keyword);
        const FileInfo fileInfo = FileInfo("test.swizzle");
        const TokenInfo info = TokenInfo(token, fileInfo);

        const Token token2 = Token("MyStruct", 0, 8, TokenType::string);
        const FileInfo fileInfo2 = FileInfo("test.swizzle");
        const TokenInfo info2 = TokenInfo(token2, fileInfo2);
    };

    TEST_FIXTURE(StructHasMemberNamedFixture, verifyHasMemberNamed)
    {
        Matcher m = Matcher().hasFieldNamed("field4");
        CHECK(!m(ast));

        m = Matcher().hasFieldNamed("field3");
        CHECK(m(ast));
    }
}
