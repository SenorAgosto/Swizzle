#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/BitfieldField.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/BitfieldSecondDotState.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct BitfieldSecondDotStateFixture
    {
        BitfieldSecondDotStateFixture()
        {
            nodeStack.push(ast.root());

            const auto bitfieldName = TokenInfo(Token("my_bitfield", 0, 11, TokenType::string), FileInfo("test.swizzle"));
            const auto bitfieldUnderlying = TokenInfo(Token("u32", 0, 3, TokenType::type), FileInfo("test.swizzle"));

            const auto node = swizzle::parser::detail::appendNode<nodes::BitfieldField>(nodeStack, bitfieldName, bitfieldUnderlying);
            nodeStack.push(node);
        }

        states::BitfieldSecondDotState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(BitfieldSecondDotStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsNumericLiteral : public BitfieldSecondDotStateFixture
    {
        const Token token = Token("2", 0, 1, TokenType::numeric_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsNumericLiteral, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::BitfieldEndPosition, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsInvalid : public BitfieldSecondDotStateFixture
    {
        const Token token = Token(";", 0, 1, TokenType::end_statement);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::SyntaxError);
    }

    struct WhenNextTokenIsNumericButTopOfStackIsNotBitfieldField : WhenNextTokenIsNumericLiteral
    {
        WhenNextTokenIsNumericButTopOfStackIsNotBitfieldField()
        {
            nodeStack.pop();
        }
    };

    TEST_FIXTURE(WhenNextTokenIsNumericButTopOfStackIsNotBitfieldField, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::ParserError);
    }
}
