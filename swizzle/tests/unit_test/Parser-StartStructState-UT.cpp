#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Matcher.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/Import.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/StartStructState.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;
    using namespace swizzle::types;
    
    struct StartStructStateFixture
    {
        StartStructStateFixture()
        {
            nodeStack.push(ast.root());

            const auto structInfo = TokenInfo(Token("struct", 0, 6, TokenType::keyword), FileInfo("test.swizzle"));
            tokenStack.push(structInfo);

            context.CurrentNamespace = "my_namespace";
        }

        states::StartStructState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        NodeStack attributeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(StartStructStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsString : public StartStructStateFixture
    {
        const Token token = Token("MyStruct", 0, 8, TokenType::string);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsString, verifyConsume)
    {
        auto matcher = Matcher().isTypeOf<nodes::Struct>().bind("struct");

        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(1U, tokenStack.size());
        CHECK(!matcher(nodeStack.top()));

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructName, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
        REQUIRE CHECK(matcher(nodeStack.top()));

        const auto& s =  matcher.bound("struct");
        const auto& node = static_cast<nodes::Struct&>(*s);

        CHECK_EQUAL("struct", node.info().token().value());
        CHECK_EQUAL("my_namespace::MyStruct", node.name());
    }

    struct WhenNextTokenIsInvalid : public StartStructStateFixture
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
