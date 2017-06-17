#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/BitfieldField.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/BitfieldStartPositionState.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct BitfieldStartPositionFixture
    {
        BitfieldStartPositionFixture()
        {
            nodeStack.push(ast.root());

            const auto bitfieldName = TokenInfo(Token("bitfield", 0, 8, TokenType::keyword), FileInfo("test.swizzle"));
            const auto bitfieldUnderlyingType = TokenInfo(Token("u16", 0, 3, TokenType::type), FileInfo("test.swizzle"));

            const auto node = swizzle::parser::detail::appendNode<nodes::BitfieldField>(nodeStack, bitfieldName, bitfieldUnderlyingType);
            nodeStack.push(node);
        }

        states::BitfieldStartPositionState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(BitfieldStartPositionFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsComma : public BitfieldStartPositionFixture
    {
        const Token token = Token(",", 0, 1, TokenType::comma);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsComma, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::BitfieldStartScope, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsDot : public BitfieldStartPositionFixture
    {
        const Token token = Token(".", 0, 1, TokenType::dot);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsDot, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::BitfieldFirstDot, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsInvalid : public BitfieldStartPositionFixture
    {
        const Token token = Token(";", 0, 1, TokenType::end_statement);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::SyntaxError);
    }
}
