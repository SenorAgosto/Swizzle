#include "./platform/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractTreeEdgeVisitorInterface.hpp>
#include <swizzle/ast/AbstractTreeVertexVisitorInterface.hpp>
#include <swizzle/ast/AbstractTreeVisitorInterface.hpp>

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/Import.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/StartEnumState.hpp>

#include <swizzle/Exceptions.hpp>

#include <array>
#include <boost/filesystem.hpp>
#include <iostream>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct StartEnumStateFixture
    {
        StartEnumStateFixture()
        {
            nodeStack.push(ast.root());

            const auto enumInfo = TokenInfo(Token("enum", 0, 3, TokenType::keyword), FileInfo("test.swizzle"));
            tokenStack.push(enumInfo);
        }

        states::StartEnumState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(StartEnumStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsString : public StartEnumStateFixture
    {
        const Token token = Token("MyEnum", 0, 6, TokenType::string);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsString, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(1U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::EnumName, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsInvalid : public StartEnumStateFixture
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
