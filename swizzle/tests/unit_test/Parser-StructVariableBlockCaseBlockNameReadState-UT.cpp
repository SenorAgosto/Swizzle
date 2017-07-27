#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Matcher.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/nodes/VariableBlock.hpp>
#include <swizzle/ast/nodes/VariableBlockCase.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/StructVariableBlockCaseBlockNameReadState.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct StructVariableBlockCaseBlockNameReadStateFixture
    {
        StructVariableBlockCaseBlockNameReadStateFixture()
        {
            nodeStack.push(ast.root());
        }

        states::StructVariableBlockCaseBlockNameReadState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        NodeStack attributeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(StructVariableBlockCaseBlockNameReadStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsColon : public StructVariableBlockCaseBlockNameReadStateFixture
    {
        const TokenInfo token = TokenInfo(Token(":", 0, 1, TokenType::colon), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsColon, verifyConsume)
    {
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(token, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructVariableBlockNamespaceFirstColonRead, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsComma : public StructVariableBlockCaseBlockNameReadStateFixture
    {
        WhenNextTokenIsComma()
        {
            const auto node = detail::appendNode<nodes::VariableBlockCase>(nodeStack);
            nodeStack.push(node);

            tokenStack.push(TokenInfo(Token(s, 0, 12, TokenType::string), FileInfo("test.swizzle", LineInfo(0, 0), LineInfo(0, 12))));
            tokenStack.push(TokenInfo(Token(s, 14, 5, TokenType::string), FileInfo("test.swizzle", LineInfo(0, 14), LineInfo(0, 19))));
            tokenStack.push(TokenInfo(Token(s, 21, 8, TokenType::string), FileInfo("test.swizzle", LineInfo(0, 21), LineInfo(0, 29))));

            context.TypeCache[s] = new nodes::Struct(info, name, "my_namespace::other");
        }

        const std::string s = "my_namespace::other::MyStruct";
        const TokenInfo info = TokenInfo(Token("struct", 0, 6, TokenType::keyword), FileInfo("test.swizzle"));
        const TokenInfo name = TokenInfo(Token("MyStruct", 0, 8, TokenType::string), FileInfo("test.swizzle"));
        const TokenInfo token = TokenInfo(Token(",", 0, 1, TokenType::comma), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsComma, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(3U, tokenStack.size());

        const auto parserState = state.consume(token, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructVariableBlockBeginCases, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        auto cases = Matcher().getChildrenOf<nodes::VariableBlockCase>().bind("cases");
        CHECK(cases(nodeStack.top()));

        const auto caseNode = cases.bound("cases_0");
        REQUIRE CHECK(caseNode);

        const auto& case0 = static_cast<nodes::VariableBlockCase&>(*caseNode);
        CHECK_EQUAL("my_namespace::other::MyStruct", case0.type().token().value());
    }

    struct WhenNextTokenIsCommaButNodeStackIsNotVariableBlockCase : public StructVariableBlockCaseBlockNameReadStateFixture
    {
        WhenNextTokenIsCommaButNodeStackIsNotVariableBlockCase()
        {
            tokenStack.push(TokenInfo(Token(s, 0, 12, TokenType::string), FileInfo("test.swizzle", LineInfo(0, 0), LineInfo(0, 12))));
            tokenStack.push(TokenInfo(Token(s, 14, 5, TokenType::string), FileInfo("test.swizzle", LineInfo(0, 14), LineInfo(0, 19))));
            tokenStack.push(TokenInfo(Token(s, 21, 8, TokenType::string), FileInfo("test.swizzle", LineInfo(0, 21), LineInfo(0, 29))));

            context.TypeCache[s] = new nodes::Struct(info, name, "my_namespace::other");
        }

        const std::string s = "my_namespace::other::MyStruct";
        const TokenInfo info = TokenInfo(Token("struct", 0, 6, TokenType::keyword), FileInfo("test.swizzle"));
        const TokenInfo name = TokenInfo(Token("MyStruct", 0, 8, TokenType::string), FileInfo("test.swizzle"));
        const TokenInfo token = TokenInfo(Token(",", 0, 1, TokenType::comma), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsCommaButNodeStackIsNotVariableBlockCase, verifyConsume)
    {
        CHECK_THROW(state.consume(token, nodeStack, attributeStack, tokenStack, context), swizzle::ParserError);
    }

    struct WhenNextTokenIsCommaButTheSymbolIsntDefined : public StructVariableBlockCaseBlockNameReadStateFixture
    {
        WhenNextTokenIsCommaButTheSymbolIsntDefined()
        {
            tokenStack.push(TokenInfo(Token(s, 0, 12, TokenType::string), FileInfo("test.swizzle", LineInfo(0, 0), LineInfo(0, 12))));
            tokenStack.push(TokenInfo(Token(s, 14, 5, TokenType::string), FileInfo("test.swizzle", LineInfo(0, 14), LineInfo(0, 19))));
            tokenStack.push(TokenInfo(Token(s, 21, 8, TokenType::string), FileInfo("test.swizzle", LineInfo(0, 21), LineInfo(0, 29))));
        }

        const std::string s = "my_namespace::other::MyStruct";
        const TokenInfo token = TokenInfo(Token(",", 0, 1, TokenType::comma), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsCommaButTheSymbolIsntDefined, verifyConsume)
    {
        CHECK_THROW(state.consume(token, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }

    struct WhenNextTokenIsInvalid : public StructVariableBlockCaseBlockNameReadStateFixture
    {
        const TokenInfo token = TokenInfo(Token(";", 0, 1, TokenType::end_statement), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(token, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }
}
