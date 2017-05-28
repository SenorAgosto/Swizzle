#include "./platform/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractTreeEdgeVisitorInterface.hpp>
#include <swizzle/ast/AbstractTreeVertexVisitorInterface.hpp>
#include <swizzle/ast/AbstractTreeVisitorInterface.hpp>

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/parser/states/UsingNameState.hpp>

#include <swizzle/Exceptions.hpp>

#include <array>
#include <boost/filesystem.hpp>
#include <iostream>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct UsingNameStateFixture
    {
        UsingNameStateFixture()
        {
            nodeStack.push(ast.root());
        }

        states::UsingNameState state;

        AbstractSyntaxTree ast;
        NodeStack nodeStack;
        TokenStack tokenStack;
    };

    TEST_FIXTURE(UsingNameStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsEqual : public UsingNameStateFixture
    {
        const Token token = Token("=", 0, 1, TokenType::equal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsEqual, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack);

        CHECK_EQUAL(ParserState::UsingReadEqual, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsInvalid : public UsingNameStateFixture
    {
        const Token token = Token(":", 0, 1, TokenType::colon);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack), swizzle::SyntaxError);
    }
}
