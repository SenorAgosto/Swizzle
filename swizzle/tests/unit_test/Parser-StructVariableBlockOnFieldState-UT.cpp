#include "./ut_support/UnitTestSupport.hpp"
#include <swizzle/parser/states/StructVariableBlockOnFieldState.hpp>

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/ast/nodes/VariableBlock.hpp>

#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/utils/AppendNode.hpp>

namespace{
    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;
    using namespace swizzle::types;

    struct StructVariableBlockOnFieldStateFixture
    {
        StructVariableBlockOnFieldStateFixture()
        {
            nodeStack.push(ast.root());

            context.CurrentNamespace = "my_namespace";

            const auto info = TokenInfo(Token("struct", 0, 6, TokenType::keyword), FileInfo("test.swizzle"));
            const auto name = TokenInfo(Token("MyStruct", 0, 8, TokenType::string), FileInfo("test.swizzle"));
            const auto node = utils::appendNode<nodes::Struct>(nodeStack, info, name, "my_namespace");
            nodeStack.push(node);

            context.TypeCache["my_namespace::MyStruct"] = node;
        }

        states::StructVariableBlockOnFieldState state;
        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        NodeStack attributeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(StructVariableBlockOnFieldStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsDot : public StructVariableBlockOnFieldStateFixture
    {
        const TokenInfo token = TokenInfo(Token(".", 0, 1, TokenType::dot), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsDot, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(token, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructVariableBlockOnNestedField, parserState);
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsLeftBrace : public StructVariableBlockOnFieldStateFixture
    {
        WhenNextTokenIsLeftBrace()
        {
            auto node = utils::appendNode<nodes::StructField>(nodeStack);
            auto& field = static_cast<nodes::StructField&>(*node);
            field.name(TokenInfo(Token("field1", 0, 6, TokenType::string), FileInfo("test.swizzle")));
            field.type("u8");

            const TokenInfo info = TokenInfo(Token("variable_block", 0, 14, TokenType::keyword), FileInfo("test.swizzle"));
            node = utils::appendNode<nodes::VariableBlock>(nodeStack, info);
            nodeStack.push(node);

            tokenStack.push(TokenInfo(Token("field1", 0, 6, TokenType::string), FileInfo("test.swizzle")));
        }

        const TokenInfo token = TokenInfo(Token("{", 0, 1, TokenType::l_brace), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsLeftBrace, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(1U, tokenStack.size());

        const auto parserState = state.consume(token, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructVariableBlockBeginCases, parserState);
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsLeftBraceAndVariableBlockFieldIsNotDefined : public StructVariableBlockOnFieldStateFixture
    {
        WhenNextTokenIsLeftBraceAndVariableBlockFieldIsNotDefined()
        {
            const TokenInfo info = TokenInfo(Token("variable_block", 0, 14, TokenType::keyword), FileInfo("test.swizzle"));
            const auto node = utils::appendNode<nodes::VariableBlock>(nodeStack, info);
            nodeStack.push(node);

            tokenStack.push(TokenInfo(Token("field1", 0, 6, TokenType::string), FileInfo("test.swizzle")));
        }

        const TokenInfo token = TokenInfo(Token("{", 0, 1, TokenType::l_brace), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsLeftBraceAndVariableBlockFieldIsNotDefined, verifyConsume)
    {
        CHECK_THROW(state.consume(token, nodeStack, attributeStack, tokenStack, context), swizzle::ParserError);
    }

    struct WhenNextTokenIsLeftBraceAndTokenStackIsEmpty : public StructVariableBlockOnFieldStateFixture
    {
        const TokenInfo token = TokenInfo(Token("{", 0, 1, TokenType::l_brace), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsLeftBraceAndTokenStackIsEmpty, verifyConsume)
    {
        CHECK_THROW(state.consume(token, nodeStack, attributeStack, tokenStack, context), swizzle::ParserError);
    }

}
