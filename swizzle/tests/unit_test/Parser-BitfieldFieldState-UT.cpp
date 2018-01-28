#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Bitfield.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/BitfieldFieldState.hpp>
#include <swizzle/types/utils/AppendNode.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;
    using namespace swizzle::types;
    
    struct BitfieldFieldStateFixture
    {
        BitfieldFieldStateFixture()
        {
            nodeStack.push(ast.root());

            const auto bitfieldInfo = TokenInfo(Token("bitfield", 0, 8, TokenType::keyword), FileInfo("test.swizzle"));
            const auto bitfieldName = TokenInfo(Token("my_bitfield", 0, 11, TokenType::string), FileInfo("test.swizzle"));

            const auto node = utils::appendNode<nodes::Bitfield>(nodeStack, bitfieldInfo, bitfieldName, "my_namespace");
            nodeStack.push(node);
        }

        states::BitfieldFieldState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        NodeStack attributeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(BitfieldFieldStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsColon : public BitfieldFieldStateFixture
    {
        const Token token = Token(":", 0, 1, TokenType::colon);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsColon, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::BitfieldFieldColonRead, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsInvalid : public BitfieldFieldStateFixture
    {
        const Token token = Token(";", 0, 1, TokenType::end_statement);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }
}
