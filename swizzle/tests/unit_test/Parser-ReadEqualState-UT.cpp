#include "./platform/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractTreeEdgeVisitorInterface.hpp>
#include <swizzle/ast/AbstractTreeVertexVisitorInterface.hpp>
#include <swizzle/ast/AbstractTreeVisitorInterface.hpp>

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/UsingReadEqualState.hpp>

#include <swizzle/Exceptions.hpp>

#include <array>
#include <boost/filesystem.hpp>
#include <iostream>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct UsingReadEqualStateFixture
    {
        UsingReadEqualStateFixture()
        {
            nodeStack.push(ast.root());
        }

        states::UsingReadEqualState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(UsingReadEqualStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsType : public UsingReadEqualStateFixture
    {
        const Token token = Token("u8", 0, 2, TokenType::type);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsType, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::UsingTypeRead, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());
    }

    struct WhenNextTokenIsString : public UsingReadEqualStateFixture
    {
        const Token token = Token("PreviousType", 0, 12, TokenType::string);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsString, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::UsingTypeRead, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());
    }

    struct WhenNextTokenIsInvalid : public UsingReadEqualStateFixture
    {
        const Token token = Token(":", 0, 1, TokenType::colon);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::SyntaxError);
    }
}
