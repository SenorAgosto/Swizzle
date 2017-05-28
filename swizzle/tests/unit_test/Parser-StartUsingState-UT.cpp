#include "./platform/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractTreeEdgeVisitorInterface.hpp>
#include <swizzle/ast/AbstractTreeVertexVisitorInterface.hpp>
#include <swizzle/ast/AbstractTreeVisitorInterface.hpp>

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/TypeAlias.hpp>
#include <swizzle/parser/states/StartUsingState.hpp>

#include <swizzle/Exceptions.hpp>

#include <array>
#include <boost/filesystem.hpp>
#include <iostream>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct StartUsingStateFixture
    {
        StartUsingStateFixture()
        {
            nodeStack.push(ast.root());
        }

        states::StartUsingState state;

        AbstractSyntaxTree ast;
        NodeStack nodeStack;
        TokenStack tokenStack;
    };

    TEST_FIXTURE(StartUsingStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsString : public StartUsingStateFixture
    {
        WhenNextTokenIsString()
        {
            tokenStack.push(TokenInfo(Token("using", 0, 5, TokenType::keyword), FileInfo("test.swizzle")));
        }
        const Token token = Token("MyType", 0, 6, TokenType::string);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsString, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(1U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack);

        CHECK_EQUAL(ParserState::UsingName, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        // TODO: validate a comment was added to the AST
    }

    struct WhenNextTokenIsInvalid : public StartUsingStateFixture
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
