#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/NamespaceValueState.hpp>

#include <cstddef>
#include <string>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    class OnNamespaceCallback
    {
    public:
        OnNamespaceCallback(std::size_t& invoked)
            : invoked_(invoked)
        {
        }

        void operator()(const std::string&)
        {
            invoked_++;
        }

    private:
        std::size_t& invoked_;
    };

    struct NamespaceValueStateFixture
    {
        NamespaceValueStateFixture()
        {
            nodeStack.push(ast.root());
        }

        std::size_t invoked = 0;

        OnNamespaceCallback callback = OnNamespaceCallback(invoked);
        states::NamespaceValueState<OnNamespaceCallback> state = states::NamespaceValueState<OnNamespaceCallback>(callback);

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(NamespaceValueStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsColon : public NamespaceValueStateFixture
    {
        const Token token = Token(":", 0, 1, TokenType::colon);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsColon, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::NamespaceFirstColon, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsEndStatement : public NamespaceValueStateFixture
    {
        WhenNextTokenIsEndStatement()
        {
            const Token t = Token("MyNamespace", 0, 11, TokenType::string);
            const FileInfo fi = FileInfo("test.swizzle");

            tokenStack.push(TokenInfo(t, fi));
        }

        const Token token = Token(";", 0, 1, TokenType::end_statement);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsEndStatement, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(1U, tokenStack.size());

        CHECK_EQUAL(0U, invoked);
        CHECK_EQUAL("", context.CurrentNamespace);

        state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        CHECK_EQUAL(1U, invoked);
        CHECK_EQUAL("MyNamespace", context.CurrentNamespace);
    }

    struct WhenNextTokenIsEndStatementAndFileHasPath : public NamespaceValueStateFixture
    {
        WhenNextTokenIsEndStatementAndFileHasPath()
        {
            const Token t1 = Token("foo", 0, 3, TokenType::string);
            const FileInfo fi1 = FileInfo("test.swizzle");

            const Token t2 = Token("bar", 0, 3, TokenType::string);
            const FileInfo fi2 = FileInfo("test.swizzle");

            const Token t3 = Token("MyNamespace", 0, 11, TokenType::string);
            const FileInfo fi3 = FileInfo("test.swizzle");

            tokenStack.push(TokenInfo(t1, fi1));
            tokenStack.push(TokenInfo(t2, fi2));
            tokenStack.push(TokenInfo(t3, fi3));
        }

        const Token token = Token(";", 0, 1, TokenType::end_statement);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsEndStatementAndFileHasPath, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(3U, tokenStack.size());

        CHECK_EQUAL(0U, invoked);
        CHECK_EQUAL("", context.CurrentNamespace);

        state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        CHECK_EQUAL(1U, invoked);
        CHECK_EQUAL("foo::bar::MyNamespace", context.CurrentNamespace);
    }

    struct WhenNextTokenIsEndStatementAndTokenStackIsEmpty : public NamespaceValueStateFixture
    {
        const Token token = Token(";", 0, 1, TokenType::end_statement);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsEndStatementAndTokenStackIsEmpty, verifyConsume)
    {
        CHECK_EQUAL(0U, tokenStack.size());
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::ParserError);
    }

    struct WhenNextTokenIsInvalid : public NamespaceValueStateFixture
    {
        const Token token = Token("blah", 0, 1, TokenType::string);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::SyntaxError);
    }
}
