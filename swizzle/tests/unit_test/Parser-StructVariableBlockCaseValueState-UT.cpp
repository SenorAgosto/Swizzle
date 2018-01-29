#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/ast/nodes/VariableBlock.hpp>
#include <swizzle/ast/nodes/VariableBlockCase.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/StructVariableBlockCaseValueState.hpp>
#include <swizzle/types/utils/AppendNode.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;
    using namespace swizzle::types;

    struct StructVariableBlockCaseValueStateFixture
    {
        StructVariableBlockCaseValueStateFixture()
        {
            nodeStack.push(ast.root());
        }

        states::StructVariableBlockCaseValueState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        NodeStack attributeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(StructVariableBlockCaseValueStateFixture, verifyConstruction)
    {
    }

    struct IntMemberFixture : public StructVariableBlockCaseValueStateFixture
    {
        IntMemberFixture()
        {
            auto node = utils::appendNode<nodes::VariableBlock>(nodeStack, info);

            auto& varBlock = static_cast<nodes::VariableBlock&>(*node);
            nodeStack.push(node);

            node = utils::appendNode<nodes::VariableBlockCase>(nodeStack);
            nodeStack.push(node);

            context.CurrentVariableOnFieldType = new nodes::StructField();
            auto& structField = static_cast<nodes::StructField&>(*context.CurrentVariableOnFieldType);
            structField.name(fieldName);
            structField.type("u8");
        }

        const Token f = Token("field1", 0, 6, TokenType::string);
        const TokenInfo fieldName = TokenInfo(f, FileInfo("test.sizzle"));

        const Token t = Token("variable_block", 0, 14, TokenType::type);
        const TokenInfo info = TokenInfo(t, FileInfo("test.swizzle"));
    };

    struct WhenNextTokenIsHexLiteral : public IntMemberFixture
    {
        const Token hex = Token("0x01", 0, 4, TokenType::hex_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");
        const TokenInfo token = TokenInfo(hex, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsHexLiteral, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(token, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructVariableBlockCaseValueRead, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        REQUIRE CHECK(utils::nodeStackTopIs<nodes::VariableBlockCase>(nodeStack));
        auto& blockCase = static_cast<nodes::VariableBlockCase&>(*nodeStack.top());

        CHECK_EQUAL("0x01", blockCase.value().token().value());
    }

    struct WhenNextTokenIsNumericLiteral : public IntMemberFixture
    {
        const TokenInfo token = TokenInfo(Token("100", 0, 4, TokenType::numeric_literal), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsNumericLiteral, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(token, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructVariableBlockCaseValueRead, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        REQUIRE CHECK(utils::nodeStackTopIs<nodes::VariableBlockCase>(nodeStack));
        auto& blockCase = static_cast<nodes::VariableBlockCase&>(*nodeStack.top());

        CHECK_EQUAL("100", blockCase.value().token().value());
    }

    struct WhenNextTokenIsCharLiteral : public IntMemberFixture
    {
        const TokenInfo token = TokenInfo(Token("'a'", 0, 3, TokenType::char_literal), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsCharLiteral, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(token, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructVariableBlockCaseValueRead, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        REQUIRE CHECK(utils::nodeStackTopIs<nodes::VariableBlockCase>(nodeStack));
        auto& blockCase = static_cast<nodes::VariableBlockCase&>(*nodeStack.top());

        CHECK_EQUAL("'a'", blockCase.value().token().value());
    }

    struct StringMemberFixture : public StructVariableBlockCaseValueStateFixture
    {
        StringMemberFixture()
        {
            auto node = utils::appendNode<nodes::VariableBlock>(nodeStack, info);

            auto& varBlock = static_cast<nodes::VariableBlock&>(*node);
            nodeStack.push(node);

            node = utils::appendNode<nodes::VariableBlockCase>(nodeStack);
            nodeStack.push(node);

            context.CurrentVariableOnFieldType = new nodes::StructField();
            auto& field = static_cast<nodes::StructField&>(*context.CurrentVariableOnFieldType);
            field.type("u8");
            field.makeArray(arraySize);
        }

        const TokenInfo info = TokenInfo(Token("variable_block", 0, 14, TokenType::type), FileInfo("test.swizzle"));
        const TokenInfo arraySize = TokenInfo(Token("10", 0, 2, TokenType::numeric_literal), FileInfo("test.swizzle"));
    };

    struct WhenNextTokenIsStringLiteral : public StringMemberFixture
    {
        const TokenInfo token = TokenInfo(Token("AS", 0, 2, TokenType::string_literal), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsStringLiteral, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(token, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructVariableBlockCaseValueRead, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        REQUIRE CHECK(utils::nodeStackTopIs<nodes::VariableBlockCase>(nodeStack));
        auto& blockCase = static_cast<nodes::VariableBlockCase&>(*nodeStack.top());

        CHECK_EQUAL("AS", blockCase.value().token().value());
    }

    struct WhenNextTokenIsStringLiteralButMemberIsInteger : public IntMemberFixture
    {
        const TokenInfo token = TokenInfo(Token("B", 0, 1, TokenType::string), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsStringLiteralButMemberIsInteger, verifyConsume)
    {
        CHECK_THROW(state.consume(token, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }

    struct WhenNextTokenIsHexLiteralButMemberIsString : public StringMemberFixture
    {
        const TokenInfo token = TokenInfo(Token("0x01", 0, 4, TokenType::hex_literal), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsHexLiteralButMemberIsString, verifyConsume)
    {
        CHECK_THROW(state.consume(token, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }

    struct WhenNextTokenIsInvalid : public StructVariableBlockCaseValueStateFixture
    {
        const TokenInfo token = TokenInfo(Token(";", 0, 1, TokenType::end_statement), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(token, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }
}
