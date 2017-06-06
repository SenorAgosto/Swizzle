#include "./platform/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Attribute.hpp>
#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/nodes/StructField.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/StructStartScopeState.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;

    struct StructStartScopeStateFixture
    {
        StructStartScopeStateFixture()
        {
            nodeStack.push(ast.root());

            const auto structKeyword = TokenInfo(Token("struct", 0, 6, TokenType::keyword), FileInfo("test.swizzle"));
            const auto name = TokenInfo(Token("MyStruct", 0, 8, TokenType::string), FileInfo("test.swizzle"));

            auto node = detail::appendNode<nodes::Struct>(nodeStack, structKeyword, name, "my_namespace");
            nodeStack.push(node);
        }

        states::StructStartScopeState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        TokenStack tokenStack;
        ParserStateContext context;
    };

    TEST_FIXTURE(StructStartScopeStateFixture, verifyConstruction)
    {
    }

    struct WhenNextTokenIsNumericLiteral : public StructStartScopeStateFixture
    {
        const Token token = Token("268", 0, 3, TokenType::numeric_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsNumericLiteral, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldLabel, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsComment : public StructStartScopeStateFixture
    {
        const Token token = Token("// comment", 0, 10, TokenType::comment);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsComment, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructStartScope, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        // TODO: validate the AST
    }

    struct WhenNextTokenIsMultilineComment : public StructStartScopeStateFixture
    {
        const Token token = Token("// comment", 0, 10, TokenType::multiline_comment);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsMultilineComment, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructStartScope, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        // TODO: validate the AST
    }

    struct WhenNextTokenIsRightBrace : public StructStartScopeStateFixture
    {
        WhenNextTokenIsRightBrace()
        {
            detail::appendNode<nodes::StructField>(nodeStack);
        }

        const Token token = Token("}", 0, 1, TokenType::r_brace);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    struct WhenNextTokenIsRightBraceAndNoFieldsWereDefined : public StructStartScopeStateFixture
    {
        const Token token = Token("}", 0, 1, TokenType::r_brace);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsRightBraceAndNoFieldsWereDefined, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::ParserError);
    }

    struct WhenNextTokenIsConst : public StructStartScopeStateFixture
    {
        const Token token = Token("const", 0, 10, TokenType::keyword);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsConst, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());
        CHECK(!context.MemberIsConst);

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructStartScope, parserState);

        CHECK(context.MemberIsConst);
        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsAttribute : public StructStartScopeStateFixture
    {
        const Token token = Token("@attribute", 0, 10, TokenType::attribute);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsAttribute, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructStartScope, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsAttributeValueAsNumericLiteral : public StructStartScopeStateFixture
    {
        WhenNextTokenIsAttributeValueAsNumericLiteral()
        {
            const Token t = Token("@attribute", 0, 10, TokenType::attribute);
            const FileInfo f = FileInfo("test.swizzle");

            const auto node = detail::appendNode<nodes::Attribute>(nodeStack, TokenInfo(t, f));
            nodeStack.push(node);
        }

        const Token token = Token("10", 0, 2, TokenType::numeric_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsAttributeValueAsNumericLiteral, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructStartScope, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        // TODO: check the AST
    }

    struct WhenNextTokenIsAttributeValueAsHexLiteral : public StructStartScopeStateFixture
    {
        WhenNextTokenIsAttributeValueAsHexLiteral()
        {
            const Token t = Token("@attribute", 0, 10, TokenType::attribute);
            const FileInfo f = FileInfo("test.swizzle");

            const auto node = detail::appendNode<nodes::Attribute>(nodeStack, TokenInfo(t, f));
            nodeStack.push(node);
        }

        const Token token = Token("0x02", 0, 4, TokenType::hex_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsAttributeValueAsHexLiteral, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructStartScope, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        // TODO: check the AST
    }

    struct WhenNextTokenIsAttributeValueAsHexLiteralButTopOfNodeStackIsNotAttribute : public StructStartScopeStateFixture
    {
        const Token token = Token("0x02", 0, 4, TokenType::hex_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsAttributeValueAsHexLiteralButTopOfNodeStackIsNotAttribute, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::ParserError);
    }

    struct WhenNextTokenIsAttributeValueAsCharLiteral : public StructStartScopeStateFixture
    {
        WhenNextTokenIsAttributeValueAsCharLiteral()
        {
            const Token t = Token("@attribute", 0, 10, TokenType::attribute);
            const FileInfo f = FileInfo("test.swizzle");

            const auto node = detail::appendNode<nodes::Attribute>(nodeStack, TokenInfo(t, f));
            nodeStack.push(node);
        }

        const Token token = Token("'a'", 0, 3, TokenType::char_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsAttributeValueAsCharLiteral, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructStartScope, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        // TODO: check the AST
    }

    struct WhenNextTokenIsAttributeValueAsCharLiteralButTopOfNodeStackIsNotAttribute : public StructStartScopeStateFixture
    {
        const Token token = Token("'a'", 0, 3, TokenType::char_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsAttributeValueAsCharLiteralButTopOfNodeStackIsNotAttribute, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::ParserError);
    }

    struct WhenNextTokenIsAttributeValueAsStringLiteral : public StructStartScopeStateFixture
    {
        WhenNextTokenIsAttributeValueAsStringLiteral()
        {
            const Token t = Token("@attribute", 0, 10, TokenType::attribute);
            const FileInfo f = FileInfo("test.swizzle");

            const auto node = detail::appendNode<nodes::Attribute>(nodeStack, TokenInfo(t, f));
            nodeStack.push(node);
        }

        const Token token = Token("blah", 0, 4, TokenType::string_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsAttributeValueAsStringLiteral, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructStartScope, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        // TODO: check the AST
    }

    struct WhenNextTokenIsAttributeValueAsStringLiteralButTopOfNodeStackIsNotAttribute : public StructStartScopeStateFixture
    {
        const Token token = Token("blah", 0, 4, TokenType::string_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsAttributeValueAsStringLiteralButTopOfNodeStackIsNotAttribute, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::ParserError);
    }

    struct WhenNextTokenIsAttributeBlock : public StructStartScopeStateFixture
    {
        WhenNextTokenIsAttributeBlock()
        {
            const Token t = Token("@attribute", 0, 10, TokenType::attribute);
            const FileInfo f = FileInfo("test.swizzle");

            const auto node = detail::appendNode<nodes::Attribute>(nodeStack, TokenInfo(t, f));
            nodeStack.push(node);
        }

        const Token token = Token("{size() != 0}", 0, 13, TokenType::attribute_block);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsAttributeBlock, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructStartScope, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        // TODO: check the AST
    }

    struct WhenNextTokenIsAttributeBlockButTopOfStackIsNotAttribute : public StructStartScopeStateFixture
    {
        const Token token = Token("{size() != 0}", 0, 13, TokenType::attribute_block);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsAttributeBlockButTopOfStackIsNotAttribute, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::ParserError);
    }

    struct WhenNextTokenIsU8 : public StructStartScopeStateFixture
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

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());

        // TODO: check the AST
    }

    struct WhenNextTokenIsI8 : public StructStartScopeStateFixture
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

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());

        // TODO: check the AST
    }

    struct WhenNextTokenIsU16 : public StructStartScopeStateFixture
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

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());

        // TODO: check the AST
    }

    struct WhenNextTokenIsI16 : public StructStartScopeStateFixture
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

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());

        // TODO: check the AST
    }

    struct WhenNextTokenIsU32 : public StructStartScopeStateFixture
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

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());

        // TODO: check the AST
    }

    struct WhenNextTokenIsI32 : public StructStartScopeStateFixture
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

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());

        // TODO: check the AST
    }

    struct WhenNextTokenIsU64 : public StructStartScopeStateFixture
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

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());

        // TODO: check the AST
    }

    struct WhenNextTokenIsI64 : public StructStartScopeStateFixture
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

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());

        // TODO: check the AST
    }

    struct WhenNextTokenIsF32 : public StructStartScopeStateFixture
    {
        const Token token = Token("f32", 0, 3, TokenType::type);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsF32, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());

        // TODO: check the AST
    }

    struct WhenNextTokenIsF64 : public StructStartScopeStateFixture
    {
        const Token token = Token("f64", 0, 3, TokenType::type);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsF64, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());

        // TODO: check the AST
    }

    struct WhenNextTokenIsBitfield : public StructStartScopeStateFixture
    {
        const Token token = Token("bitfield", 0, 8, TokenType::type);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsBitfield, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, tokenStack, context), swizzle::SyntaxError);
    }

    struct WhenNextTokenIsUserDefinedTypeWithoutNamespace : public StructStartScopeStateFixture
    {
        const Token token = Token("MyType", 0, 6, TokenType::string);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsUserDefinedTypeWithoutNamespace, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());

        // TODO: check the AST
    }

    struct WhenNextTokenIsNamespace : public StructStartScopeStateFixture
    {
        const Token token = Token("my_namespace", 0, 12, TokenType::string);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsNamespace, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());

        // TODO: check the AST
    }

    struct WhenNextTokenIsMemberName : public StructStartScopeStateFixture
    {
        WhenNextTokenIsMemberName()
        {
            const auto node = detail::appendNode<nodes::StructField>(nodeStack);
            nodeStack.push(node);
        }

        const Token token = Token("field1", 0, 6, TokenType::string);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsMemberName, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldName, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
    }

    struct WhenNextTokenIsInvalid : public StructStartScopeStateFixture
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
