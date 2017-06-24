#include "./ut_support/UnitTestSupport.hpp"
#include <swizzle/parser/detail/CreateMember.hpp>

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/TokenStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct CreateMemberFixture
    {
        CreateMemberFixture()
        {
            nodeStack.push(ast.root());

            const auto info = TokenInfo(Token("struct", 0, 6, TokenType::keyword), FileInfo("test.swizzle"));
            const auto name = TokenInfo(Token("MyStruct", 0, 8, TokenType::string), FileInfo("test.swizzle"));

            auto node = detail::appendNode<nodes::Struct>(nodeStack, info, name, "my_namespace");
            nodeStack.push(node);

            node = detail::appendNode<nodes::StructField>(nodeStack);
            auto& field1 = static_cast<nodes::StructField&>(*node);
            field1.name(TokenInfo(Token("field1", 0, 6, TokenType::string), FileInfo("test.swizzle")));
            field1.type("u8");

            node = detail::appendNode<nodes::StructField>(nodeStack);
            auto& field2 = static_cast<nodes::StructField&>(*node);
            field2.name(TokenInfo(Token("field2", 0, 6, TokenType::string), FileInfo("test.swizzle")));
            field2.type("u32");

            node = detail::appendNode<nodes::StructField>(nodeStack);
            auto& field3 = static_cast<nodes::StructField&>(*node);
            field3.name(TokenInfo(Token("field3", 0, 6, TokenType::string), FileInfo("test.swizzle")));
            field3.type("i8");

            nodeStack.push(node);
        }

        AbstractSyntaxTree ast;
        NodeStack nodeStack;
        TokenStack tokenStack;
        ParserStateContext context;

        const TokenInfo currentToken = TokenInfo(Token("field3", 0, 6, TokenType::string), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(CreateMemberFixture, verifyCreateMember)
    {
        // push one token onto the tokenStack, the name of a field in the current struct which is an integer type.
        tokenStack.push(TokenInfo(Token("field2", 0, 6, TokenType::string), FileInfo("test.swizzle")));

        const auto info = detail::createMember(currentToken, nodeStack, tokenStack, context);
        CHECK_EQUAL("field2", info.token().to_string());
    }

    struct WhenVectorSizeMemberIsNested
    {
        WhenVectorSizeMemberIsNested()
        {
            nodeStack.push(ast.root());

            const auto info = TokenInfo(Token("struct", 0, 6, TokenType::keyword), FileInfo("test.swizzle"));
            const auto name = TokenInfo(Token("MyStruct", 0, 8, TokenType::string), FileInfo("test.swizzle"));

            auto node = detail::appendNode<nodes::Struct>(nodeStack, info, name, "my_namespace");
            nodeStack.push(node);

            node = detail::appendNode<nodes::StructField>(nodeStack);
            auto& field1 = static_cast<nodes::StructField&>(*node);
            field1.name(TokenInfo(Token("field1", 0, 6, TokenType::string), FileInfo("test.swizzle")));
            field1.type("u8");

            node = detail::appendNode<nodes::StructField>(nodeStack);
            auto& field2 = static_cast<nodes::StructField&>(*node);
            field2.name(TokenInfo(Token("field2", 0, 6, TokenType::string), FileInfo("test.swizzle")));
            field2.type("u32");

            // add the struct to the the type cache
            context.TypeCache["my_namespace::MyStruct"] = nodeStack.top();
            nodeStack.pop();

            const auto info2 = TokenInfo(Token("struct", 0, 6, TokenType::keyword), FileInfo("test.swizzle"));
            const auto name2 = TokenInfo(Token("MyStruct2", 0, 8, TokenType::string), FileInfo("test.swizzle"));

            node = detail::appendNode<nodes::Struct>(nodeStack, info2, name2, "my_namespace");
            nodeStack.push(node);

            node = detail::appendNode<nodes::StructField>(nodeStack);
            auto& field3 = static_cast<nodes::StructField&>(*node);
            field3.name(TokenInfo(Token("field1", 0, 6, TokenType::string), FileInfo("test.swizzle")));
            field3.type("MyStruct");

            nodeStack.push(node);
        }

        AbstractSyntaxTree ast;
        NodeStack nodeStack;
        TokenStack tokenStack;
        ParserStateContext context;

        const std::string s = "field1.field2";
        const TokenInfo currentToken = TokenInfo(Token("field3", 0, 6, TokenType::string), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenVectorSizeMemberIsNested, verifyCreateMember)
    {
        tokenStack.push(TokenInfo(Token(s, 0, 6, TokenType::string), FileInfo("test.swizzle", LineInfo(1, 1), LineInfo(1, 7))));
        tokenStack.push(TokenInfo(Token(s, 7, 6, TokenType::string), FileInfo("test.swizzle", LineInfo(1, 8), LineInfo(1, 14))));

        const auto info = detail::createMember(currentToken, nodeStack, tokenStack, context);
        CHECK_EQUAL("field1.field2", info.token().to_string());
    }

    struct WhenTokenStackIsEmpty
    {
        NodeStack nodeStack;
        TokenStack tokenStack;
        ParserStateContext context;

        TokenInfo token = TokenInfo(Token("current", 0, 7, TokenType::string), FileInfo("test.swizzle"));
    };

    TEST_FIXTURE(WhenTokenStackIsEmpty, verifyCreateMemberThrows)
    {
        CHECK_THROW(detail::createMember(token, nodeStack, tokenStack, context), swizzle::SyntaxError);
    }
}
