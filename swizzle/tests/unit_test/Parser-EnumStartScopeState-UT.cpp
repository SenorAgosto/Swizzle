#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Matcher.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/Enum.hpp>
#include <swizzle/ast/nodes/EnumField.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/EnumStartScopeState.hpp>
#include <swizzle/types/utils/AppendNode.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;
    using namespace swizzle::types;
    
    struct EnumStartScopeStateFixture
    {
        EnumStartScopeStateFixture()
        {
            nodeStack.push(ast.root());

            const auto enumInfo = TokenInfo(Token("enum", 0, 3, TokenType::keyword), FileInfo("test.swizzle"));
            const auto enumName = TokenInfo(Token("my_enum", 0, 7, TokenType::string), FileInfo("test.swizzle"));

            const auto node = utils::appendNode<nodes::Enum>(nodeStack, enumInfo, enumName, "my_namespace");
            nodeStack.push(node);
        }

        states::EnumStartScopeState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        NodeStack attributeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(EnumStartScopeStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsComment : public EnumStartScopeStateFixture
    {
        const Token token = Token("// comment", 0, 10, TokenType::comment);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsComment, verifyConsume)
    {
        auto matcher = Matcher().hasChildOf<nodes::Comment>().bind("comment");

        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());
        CHECK(!matcher(nodeStack.top()));

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::EnumStartScope, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        REQUIRE CHECK(matcher(nodeStack.top()));
        REQUIRE CHECK(matcher.bound("comment"));
    }

    struct WhenNextTokenIsMultilineComment : public EnumStartScopeStateFixture
    {
        const Token token = Token("// comment \\\n comment2", 0, 21, TokenType::multiline_comment);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsMultilineComment, verifyConsume)
    {
        auto matcher = Matcher().hasChildOf<nodes::MultilineComment>().bind("comment");

        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());
        CHECK(!matcher(nodeStack.top()));

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::EnumStartScope, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        REQUIRE CHECK(matcher(nodeStack.top()));
        REQUIRE CHECK(matcher.bound("comment"));
    }

    struct WhenNextTokenIsFieldName : public EnumStartScopeStateFixture
    {
        const Token token = Token("field1", 0, 6, TokenType::string);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsFieldName, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::EnumField, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        CHECK(utils::nodeStackTopIs<nodes::EnumField>(nodeStack));
    }

    struct WhenNextTokenIsRightBrace : public EnumStartScopeStateFixture
    {
        WhenNextTokenIsRightBrace()
        {
            auto& top = dynamic_cast<nodes::Enum&>(*nodeStack.top());
            top.append(new Node());
        }

        const Token token = Token("}", 0, 1, TokenType::r_brace);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsRightBrace, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);

        REQUIRE CHECK_EQUAL(1U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsInvalid : public EnumStartScopeStateFixture
    {
        const Token token = Token(";", 0, 1, TokenType::end_statement);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }

    struct WhenNextTokenIsFieldNameAndTopOfStackIsNotEnum : public EnumStartScopeStateFixture
    {
        WhenNextTokenIsFieldNameAndTopOfStackIsNotEnum()
        {
            nodeStack.pop();
        }

        const Token token = Token("field1", 0, 6, TokenType::string);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsFieldNameAndTopOfStackIsNotEnum, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::ParserError);
    }

    struct WhenNextTokenIsRightBraceAndTopOfStackIsNotEnum : public EnumStartScopeStateFixture
    {
        WhenNextTokenIsRightBraceAndTopOfStackIsNotEnum()
        {
            nodeStack.pop();
        }

        const Token token = Token("}", 0, 1, TokenType::r_brace);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsRightBraceAndTopOfStackIsNotEnum, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::ParserError);
    }

    struct WhenNextTokenIsRightBraceButNoFieldsWereCreated : public EnumStartScopeStateFixture
    {
        const Token token = Token("}", 0, 1, TokenType::r_brace);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsRightBraceButNoFieldsWereCreated, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }
}
