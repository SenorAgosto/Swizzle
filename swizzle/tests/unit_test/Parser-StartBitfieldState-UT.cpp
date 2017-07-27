#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/Import.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/StartBitfieldState.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct StartBitfieldStateFixture
    {
        StartBitfieldStateFixture()
        {
            nodeStack.push(ast.root());

            const auto bitfieldInfo = TokenInfo(Token("bitfield", 0, 8, TokenType::keyword), FileInfo("test.swizzle"));
            tokenStack.push(bitfieldInfo);
        }

        states::StartBitfieldState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        NodeStack attributeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(StartBitfieldStateFixture, verifyConstruction)
    {
    }


    struct WhenNextTokenIsString : public StartBitfieldStateFixture
    {
        const Token token = Token("MyBitfield", 0, 10, TokenType::string);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsString, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(1U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::BitfieldName, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsInvalid : public StartBitfieldStateFixture
    {
        const Token token = Token(":", 0, 1, TokenType::colon);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
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
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::ParserError);
    }
}
