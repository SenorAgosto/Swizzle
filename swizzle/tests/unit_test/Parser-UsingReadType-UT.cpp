#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/TypeAlias.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/states/UsingTypeReadState.hpp>

#include <boost/utility/string_view.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct UsingTypeReadStateFixture
    {
        UsingTypeReadStateFixture()
        {
            nodeStack.push(ast.root());

            const auto node = swizzle::parser::detail::appendNode<nodes::TypeAlias>(nodeStack, TokenInfo(Token(), FileInfo("test.swizzle")), TokenInfo(Token(), FileInfo("test.swizzle")));
            nodeStack.push(node);
        }

        states::UsingTypeReadState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(UsingTypeReadStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsColon : public UsingTypeReadStateFixture
    {
        const Token token = Token(":", 0, 1, TokenType::colon);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsColon, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::UsingFirstColon, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsEndStatement : public UsingTypeReadStateFixture
    {
        WhenNextTokenIsEndStatement()
        {
            tokenStack.emplace(Token(sv, 0, 3, TokenType::string), FileInfo("test.swizzle", LineInfo(1, 0), LineInfo(1, 4)));
            tokenStack.emplace(Token(sv, 6, 3, TokenType::string), FileInfo("test.swizzle", LineInfo(1, 6), LineInfo(1, 9)));
            tokenStack.emplace(Token(sv, 10, 6, TokenType::string), FileInfo("test.swizzle", LineInfo(1, 11), LineInfo(1, 17)));
        }

        const boost::string_view sv = "foo::bar::MyType";

        const Token token = Token(";", 0, 1, TokenType::end_statement);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsEndStatement, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(3U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsInvalid : public UsingTypeReadStateFixture
    {
        const Token token = Token("=", 0, 1, TokenType::equal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::SyntaxError);
    }

    struct WhenTopOfNodeStackIsNotTypeAlias : public UsingTypeReadStateFixture
    {
        WhenTopOfNodeStackIsNotTypeAlias()
        {
            nodeStack.pop();

            Node::smartptr node = new nodes::Comment(TokenInfo());
            nodeStack.push(node);
        }

        const Token token = Token("=", 0, 1, TokenType::equal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenTopOfNodeStackIsNotTypeAlias, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::ParserError);
    }
}
