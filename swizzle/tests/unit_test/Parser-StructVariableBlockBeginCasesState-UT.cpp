#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Matcher.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/ast/nodes/VariableBlock.hpp>
#include <swizzle/ast/nodes/VariableBlockCase.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/StructVariableBlockBeginCasesState.hpp>
#include <swizzle/types/utils/AppendNode.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;
    using namespace swizzle::types;

    struct StructVariableBlockBeginCasesStateFixture
    {
        StructVariableBlockBeginCasesStateFixture()
        {
            nodeStack.push(ast.root());
        }

        states::StructVariableBlockBeginCasesState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        NodeStack attributeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(StructVariableBlockBeginCasesStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsComment : public StructVariableBlockBeginCasesStateFixture
    {
        const TokenInfo token = TokenInfo(Token("// comment", 0, 10, TokenType::comment), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsComment, verifyConsume)
    {
        auto matcher = Matcher().hasChildOf<nodes::Comment>();

        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());
        CHECK(!matcher(nodeStack.top()));

        const auto parserState = state.consume(token, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructVariableBlockBeginCases, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
        CHECK(matcher(nodeStack.top()));
    }

    struct WhenNextTokenIsMultilineComment : public StructVariableBlockBeginCasesStateFixture
    {
        const TokenInfo token = TokenInfo(Token("// comment", 0, 10, TokenType::multiline_comment), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsMultilineComment, verifyConsume)
    {
        auto matcher = Matcher().hasChildOf<nodes::MultilineComment>();

        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());
        CHECK(!matcher(nodeStack.top()));

        const auto parserState = state.consume(token, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructVariableBlockBeginCases, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
        CHECK(matcher(nodeStack.top()));
    }

    struct WhenNextTokenIsCase : public StructVariableBlockBeginCasesStateFixture
    {
        const TokenInfo token = TokenInfo(Token("case", 0, 4, TokenType::keyword), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsCase, verifyConsume)
    {
        auto matcher = Matcher().hasChildOf<nodes::VariableBlockCase>();

        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());
        CHECK(!matcher(nodeStack.top()));

        const auto parserState = state.consume(token, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructVariableBlockCaseValue, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
        CHECK(utils::nodeStackTopIs<nodes::VariableBlockCase>(nodeStack));
    }

    struct WhenNextTokenIsRightBrace : public StructVariableBlockBeginCasesStateFixture
    {
        WhenNextTokenIsRightBrace()
        {
            const auto info = TokenInfo(Token("variable_block", 0, 14, TokenType::type), FileInfo("test.swizzle"));
            const auto node = utils::appendNode<nodes::VariableBlock>(nodeStack, info);

            nodeStack.push(node);

            utils::appendNode<nodes::VariableBlockCase>(nodeStack);
        }

        const TokenInfo token = TokenInfo(Token("}", 0, 1, TokenType::r_brace), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsRightBrace, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(token, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructStartScope, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsRightBraceAndVariableBlockIsEmpty : public StructVariableBlockBeginCasesStateFixture
    {
        const TokenInfo token = TokenInfo(Token("}", 0, 1, TokenType::r_brace), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsRightBraceAndVariableBlockIsEmpty, verifyConsume)
    {
        CHECK_THROW(state.consume(token, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }
}
