#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/Import.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/StartStructState.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct StartStructStateFixture
    {
        StartStructStateFixture()
        {
            nodeStack.push(ast.root());

            const auto structInfo = TokenInfo(Token("struct", 0, 6, TokenType::keyword), FileInfo("test.swizzle"));
            tokenStack.push(structInfo);
        }

        states::StartStructState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(StartStructStateFixture, verifyConstruction)
    {
    }


    struct WhenNextTokenIsString : public StartStructStateFixture
    {
        const Token token = Token("MyStruct", 0, 8, TokenType::string);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsString, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(1U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructName, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        // TODO: verify AST
    }

    struct WhenNextTokenIsInvalid : public StartStructStateFixture
    {
        const Token token = Token(":", 0, 1, TokenType::colon);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::SyntaxError);
    }

    struct WhenNextTokenIsStringButTokenStackIsEmpty : public WhenNextTokenIsString
    {
        WhenNextTokenIsStringButTokenStackIsEmpty()
        {
            tokenStack.pop();
        }
    };

    TEST_FIXTURE(WhenNextTokenIsStringButTokenStackIsEmpty, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::ParserError);
    }
}
