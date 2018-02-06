#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Bitfield.hpp>
#include <swizzle/ast/nodes/BitfieldField.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/BitfieldFieldColonReadState.hpp>
#include <swizzle/types/utils/AppendNode.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;
    using namespace swizzle::types;

    struct BitfieldFieldColonReadStateFixture
    {
        BitfieldFieldColonReadStateFixture()
        {
            nodeStack.push(ast.root());
            const auto bitfieldInfo = TokenInfo(Token("bitfield", 0, 8, TokenType::keyword), FileInfo("test.swizzle"));
            const auto bitfieldName = TokenInfo(Token("bf", 0, 2, TokenType::string), FileInfo("test.swizzle"));
            const auto bitfieldUnderlyingType = TokenInfo(Token("u16", 0, 3, TokenType::type), FileInfo("test.swizzle"));
            const auto underlying = TokenInfo(Token("u8", 0, 2, TokenType::type), FileInfo("test.swizzle"));
            
            const auto bf_node = utils::appendNode<nodes::Bitfield>(nodeStack, bitfieldInfo, bitfieldName, "foo");
            auto& bf = dynamic_cast<nodes::Bitfield&>(*bf_node);
            
            bf.underlying(underlying);
            nodeStack.push(bf_node);
            
            const auto node = utils::appendNode<nodes::BitfieldField>(nodeStack, bitfieldName, bitfieldUnderlyingType);
            nodeStack.push(node);
        }

        states::BitfieldFieldColonReadState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        NodeStack attributeStack;
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
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::BitfieldStartPosition, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        REQUIRE CHECK(utils::nodeStackTopIs<nodes::BitfieldField>(nodeStack));

        auto& field = static_cast<nodes::BitfieldField&>(*nodeStack.top());
        CHECK_EQUAL(0, field.beginBit());
    }

    struct WhenNextTokenIsInvalid : public BitfieldFieldColonReadStateFixture
    {
        const Token token = Token(";", 0, 1, TokenType::end_statement);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }

    struct WhenBitfieldHasInvalidUnderlyingType
    {
        WhenBitfieldHasInvalidUnderlyingType()
        {
            nodeStack.push(ast.root());

            const auto bitfieldName = TokenInfo(Token("bitfield", 0, 8, TokenType::keyword), FileInfo("test.swizzle"));
            const auto bitfieldUnderlyingType = TokenInfo(Token("i16", 0, 3, TokenType::type), FileInfo("test.swizzle"));

            const auto node = utils::appendNode<nodes::BitfieldField>(nodeStack, bitfieldName, bitfieldUnderlyingType);
            nodeStack.push(node);
        }

        states::BitfieldFieldColonReadState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        NodeStack attributeStack;
        TokenStack tokenStack;
        ParserStateContext context;

        const Token token = Token("0", 0, 1, TokenType::numeric_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenBitfieldHasInvalidUnderlyingType, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::ParserError);
    }
}
