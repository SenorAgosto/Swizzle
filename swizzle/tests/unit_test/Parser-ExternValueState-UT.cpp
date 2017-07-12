#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/ExternValueState.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct ExternValueStateFixture
    {
        ExternValueStateFixture()
        {
            nodeStack.push(ast.root());
        }

        states::ExternValueState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(ExternValueStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsColon : public ExternValueStateFixture
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

        CHECK_EQUAL(ParserState::ExternFirstColon, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsEndStatement : public ExternValueStateFixture
    {
        WhenNextTokenIsEndStatement()
        {
            const Token t = Token("MyType", 0, 6, TokenType::string);
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

        state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        // TODO: check AST
    }

    struct WhenNextTokenIsEndStatementAndTokenStackIsEmpty : public ExternValueStateFixture
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

    struct WhenNextTokenIsInvalid : public ExternValueStateFixture
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
