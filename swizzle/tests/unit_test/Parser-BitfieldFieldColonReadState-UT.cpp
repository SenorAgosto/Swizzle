#include "./platform/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/BitfieldField.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/BitfieldFieldColonReadState.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct BitfieldFieldColonReadStateFixture
    {
        BitfieldFieldColonReadStateFixture()
        {
            nodeStack.push(ast.root());

            const auto bitfieldName = TokenInfo(Token("bitfield", 0, 8, TokenType::keyword), FileInfo("test.swizzle"));
            const auto bitfieldUnderlyingType = TokenInfo(Token("u16", 0, 3, TokenType::type), FileInfo("test.swizzle"));

            const auto node = swizzle::parser::detail::appendNode<nodes::BitfieldField>(nodeStack, bitfieldName, bitfieldUnderlyingType);
            nodeStack.push(node);
        }

        states::BitfieldFieldColonReadState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(BitfieldFieldColonReadStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsBit0 : public BitfieldFieldColonReadStateFixture
    {
        const Token token = Token("0", 0, 1, TokenType::numeric_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsBit0, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::BitfieldStartPosition, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        // TODO: check the ast & start bit value
    }

    struct WhenNextTokenIsInvalid : public BitfieldFieldColonReadStateFixture
    {
        const Token token = Token(";", 0, 1, TokenType::end_statement);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::SyntaxError);
    }

    struct WhenBitfieldHasInvalidUnderlyingType
    {
        WhenBitfieldHasInvalidUnderlyingType()
        {
            nodeStack.push(ast.root());

            const auto bitfieldName = TokenInfo(Token("bitfield", 0, 8, TokenType::keyword), FileInfo("test.swizzle"));
            const auto bitfieldUnderlyingType = TokenInfo(Token("i16", 0, 3, TokenType::type), FileInfo("test.swizzle"));

            const auto node = swizzle::parser::detail::appendNode<nodes::BitfieldField>(nodeStack, bitfieldName, bitfieldUnderlyingType);
            nodeStack.push(node);
        }

        states::BitfieldFieldColonReadState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        TokenStack tokenStack;
        ParserStateContext context;

        const Token token = Token("0", 0, 1, TokenType::numeric_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenBitfieldHasInvalidUnderlyingType, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::ParserError);
    }
}
