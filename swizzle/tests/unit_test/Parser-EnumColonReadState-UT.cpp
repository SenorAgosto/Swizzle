#include "./platform/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractTreeEdgeVisitorInterface.hpp>
#include <swizzle/ast/AbstractTreeVertexVisitorInterface.hpp>
#include <swizzle/ast/AbstractTreeVisitorInterface.hpp>

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Enum.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/EnumColonReadState.hpp>

#include <swizzle/Exceptions.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct EnumColonReadStateFixture
    {
        EnumColonReadStateFixture()
        {
            nodeStack.push(ast.root());

            const auto enumInfo = TokenInfo(Token("my_enum", 0, 3, TokenType::keyword), FileInfo("test.swizzle"));
            const auto enumName = TokenInfo(Token("my_enum", 0, 7, TokenType::string), FileInfo("test.swizzle"));

            const auto node = swizzle::parser::detail::appendNode<nodes::Enum>(nodeStack, enumInfo, enumName, "my_namespace");
            nodeStack.push(node);
        }

        states::EnumColonReadState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(EnumColonReadStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsU8 : public EnumColonReadStateFixture
    {
        const Token token = Token("u8", 0, 2, TokenType::type);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsU8, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::EnumUnderlyingType, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsI8 : public EnumColonReadStateFixture
    {
        const Token token = Token("i8", 0, 2, TokenType::type);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsI8, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::EnumUnderlyingType, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsU16 : public EnumColonReadStateFixture
    {
        const Token token = Token("u16", 0, 3, TokenType::type);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsU16, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::EnumUnderlyingType, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsI16 : public EnumColonReadStateFixture
    {
        const Token token = Token("i16", 0, 3, TokenType::type);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsI16, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::EnumUnderlyingType, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsU32 : public EnumColonReadStateFixture
    {
        const Token token = Token("u32", 0, 3, TokenType::type);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsU32, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::EnumUnderlyingType, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsI32 : public EnumColonReadStateFixture
    {
        const Token token = Token("i32", 0, 3, TokenType::type);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsI32, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::EnumUnderlyingType, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsU64 : public EnumColonReadStateFixture
    {
        const Token token = Token("u64", 0, 3, TokenType::type);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsU64, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::EnumUnderlyingType, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsI64 : public EnumColonReadStateFixture
    {
        const Token token = Token("i64", 0, 3, TokenType::type);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsI64, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::EnumUnderlyingType, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsInvalid : public EnumColonReadStateFixture
    {
        const Token token = Token(";", 0, 1, TokenType::end_statement);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::SyntaxError);
    }

    struct WhenNodeStackDoesntHaveEnumNodeOnTop : public EnumColonReadStateFixture
    {
        WhenNodeStackDoesntHaveEnumNodeOnTop()
        {
            nodeStack.pop();
        }

        const Token token = Token("i64", 0, 3, TokenType::type);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNodeStackDoesntHaveEnumNodeOnTop, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::ParserError);
    }
}
