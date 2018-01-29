#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/Matcher.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Attribute.hpp>
#include <swizzle/ast/nodes/AttributeBlock.hpp>
#include <swizzle/ast/nodes/CharLiteral.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/HexLiteral.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/ast/nodes/NumericLiteral.hpp>
#include <swizzle/ast/nodes/StringLiteral.hpp>
#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/ast/nodes/VariableBlock.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/states/StructStartScopeState.hpp>
#include <swizzle/types/utils/AppendNode.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>

namespace {

    using namespace swizzle::ast;
    using namespace swizzle::lexer;
    using namespace swizzle::parser;
    using namespace swizzle::types;
    
    struct StructStartScopeStateFixture
    {
        StructStartScopeStateFixture()
        {
            nodeStack.push(ast.root());

            const auto structKeyword = TokenInfo(Token("struct", 0, 6, TokenType::keyword), FileInfo("test.swizzle"));
            const auto name = TokenInfo(Token("MyStruct", 0, 8, TokenType::string), FileInfo("test.swizzle"));

            auto node = utils::appendNode<nodes::Struct>(nodeStack, structKeyword, name, "my_namespace");
            nodeStack.push(node);
        }

        states::StructStartScopeState state;

        AbstractSyntaxTree ast;

        NodeStack nodeStack;
        NodeStack attributeStack;
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldLabel, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, attributeStack.size());
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
        auto matcher = Matcher().hasChildOf<nodes::Comment>();

        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());
        CHECK(!matcher(nodeStack.top()));

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructStartScope, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
        CHECK(matcher(nodeStack.top()));
    }

    struct WhenNextTokenIsMultilineComment : public StructStartScopeStateFixture
    {
        const Token token = Token("// comment", 0, 10, TokenType::multiline_comment);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsMultilineComment, verifyConsume)
    {
        auto matcher = Matcher().hasChildOf<nodes::MultilineComment>();

        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());
        CHECK(!matcher(nodeStack.top()));

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructStartScope, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
        CHECK(matcher(nodeStack.top()));
    }

    struct WhenNextTokenIsRightBrace : public StructStartScopeStateFixture
    {
        WhenNextTokenIsRightBrace()
        {
            utils::appendNode<nodes::StructField>(nodeStack);
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

    struct WhenNextTokenIsRightBraceAndNoFieldsWereDefined : public StructStartScopeStateFixture
    {
        const Token token = Token("}", 0, 1, TokenType::r_brace);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsRightBraceAndNoFieldsWereDefined, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());
        
        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);
        
        CHECK_EQUAL(ParserState::TranslationUnitMain, parserState);
        
        CHECK_EQUAL(1U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());
        CHECK(!context.MemberIsConst);

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructStartScope, parserState);

        CHECK(context.MemberIsConst);
        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructStartScope, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        auto matcher = Matcher().isTypeOf<nodes::Attribute>();
        CHECK(matcher(attributeStack.top()));
    }

    struct WhenNextTokenIsAttributeValueAsNumericLiteral : public StructStartScopeStateFixture
    {
        WhenNextTokenIsAttributeValueAsNumericLiteral()
        {
            const Token t = Token("@attribute", 0, 10, TokenType::attribute);
            const FileInfo f = FileInfo("test.swizzle");

            attributeStack.push(new nodes::Attribute(TokenInfo(t, f)));
        }

        const Token token = Token("10", 0, 2, TokenType::numeric_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");
        const TokenInfo info = TokenInfo(token, fileInfo);

        const Token token2 = Token("=", 0, 1, TokenType::equal);
        const FileInfo fileInfo2 = FileInfo("test.swizzle");
        const TokenInfo equal = TokenInfo(token2, fileInfo2);
    };

    TEST_FIXTURE(WhenNextTokenIsAttributeValueAsNumericLiteral, verifyConsume)
    {
        auto parserState = state.consume(equal, nodeStack, attributeStack, tokenStack, context);
        CHECK_EQUAL(ParserState::StructStartScope, parserState);

        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(1U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructStartScope, parserState);

        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(1U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        REQUIRE CHECK(utils::nodeStackTopIs<nodes::Attribute>(attributeStack));
        const auto attributeNode = attributeStack.top();
        REQUIRE CHECK(attributeNode);

        auto attributeValueMatcher = Matcher().getChildrenOf<nodes::NumericLiteral>().bind("value");
        REQUIRE CHECK(attributeValueMatcher(attributeNode));

        const auto valueNode = attributeValueMatcher.bound("value_0");
        REQUIRE CHECK(valueNode);

        const auto& value = static_cast<nodes::NumericLiteral&>(*valueNode);
        CHECK_EQUAL("10", value.info().token().value());
    }

    struct WhenNextTokenIsAttributeValueAsHexLiteral : public StructStartScopeStateFixture
    {
        WhenNextTokenIsAttributeValueAsHexLiteral()
        {
            const Token t = Token("@attribute", 0, 10, TokenType::attribute);
            const FileInfo f = FileInfo("test.swizzle");

            attributeStack.push(new nodes::Attribute(TokenInfo(t, f)));
        }

        const Token token = Token("0x02", 0, 4, TokenType::hex_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");
        const TokenInfo info = TokenInfo(token, fileInfo);

        const Token token2 = Token("=", 0, 1, TokenType::equal);
        const FileInfo fileInfo2 = FileInfo("test.swizzle");
        const TokenInfo equal = TokenInfo(token2, fileInfo2);
    };

    TEST_FIXTURE(WhenNextTokenIsAttributeValueAsHexLiteral, verifyConsume)
    {
        auto parserState = state.consume(equal, nodeStack, attributeStack, tokenStack, context);
        CHECK_EQUAL(ParserState::StructStartScope, parserState);

        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(1U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructStartScope, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        REQUIRE CHECK(utils::nodeStackTopIs<nodes::Attribute>(attributeStack));

        const auto attributeNode = attributeStack.top();
        REQUIRE CHECK(attributeNode);

        auto attributeValueMatcher = Matcher().getChildrenOf<nodes::HexLiteral>().bind("value");
        REQUIRE CHECK(attributeValueMatcher(attributeNode));

        const auto valueNode = attributeValueMatcher.bound("value_0");
        REQUIRE CHECK(valueNode);

        const auto& value = static_cast<nodes::HexLiteral&>(*valueNode);
        CHECK_EQUAL("0x02", value.info().token().value());
    }

    struct WhenNextTokenIsAttributeValueAsHexLiteralButTopOfNodeStackIsNotAttribute : public StructStartScopeStateFixture
    {
        const Token token = Token("0x02", 0, 4, TokenType::hex_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsAttributeValueAsHexLiteralButTopOfNodeStackIsNotAttribute, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::ParserError);
    }

    struct WhenNextTokenIsAttributeValueAsCharLiteral : public StructStartScopeStateFixture
    {
        WhenNextTokenIsAttributeValueAsCharLiteral()
        {
            const Token t = Token("@attribute", 0, 10, TokenType::attribute);
            const FileInfo f = FileInfo("test.swizzle");

            attributeStack.push(new nodes::Attribute(TokenInfo(t, f)));
        }

        const Token token = Token("'a'", 0, 3, TokenType::char_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");
        const TokenInfo info = TokenInfo(token, fileInfo);

        const Token token2 = Token("=", 0, 1, TokenType::equal);
        const FileInfo fileInfo2 = FileInfo("test.swizzle");
        const TokenInfo equal = TokenInfo(token2, fileInfo2);
    };

    TEST_FIXTURE(WhenNextTokenIsAttributeValueAsCharLiteral, verifyConsume)
    {
        auto parserState = state.consume(equal, nodeStack, attributeStack, tokenStack, context);
        CHECK_EQUAL(ParserState::StructStartScope, parserState);

        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(1U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructStartScope, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        REQUIRE CHECK(utils::nodeStackTopIs<nodes::Attribute>(attributeStack));
        const auto attributeNode = attributeStack.top();

        auto attributeValueMatcher = Matcher().getChildrenOf<nodes::CharLiteral>().bind("value");
        REQUIRE CHECK(attributeValueMatcher(attributeNode));

        const auto valueNode = attributeValueMatcher.bound("value_0");
        REQUIRE CHECK(valueNode);

        const auto& value = static_cast<nodes::CharLiteral&>(*valueNode);
        CHECK_EQUAL("'a'", value.info().token().value());
    }

    struct WhenNextTokenIsAttributeValueAsCharLiteralButTopOfNodeStackIsNotAttribute : public StructStartScopeStateFixture
    {
        const Token token = Token("'a'", 0, 3, TokenType::char_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsAttributeValueAsCharLiteralButTopOfNodeStackIsNotAttribute, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::ParserError);
    }

    struct WhenNextTokenIsAttributeValueAsStringLiteral : public StructStartScopeStateFixture
    {
        WhenNextTokenIsAttributeValueAsStringLiteral()
        {
            const Token t = Token("@attribute", 0, 10, TokenType::attribute);
            const FileInfo f = FileInfo("test.swizzle");

            attributeStack.push(new nodes::Attribute(TokenInfo(t, f)));
        }

        const Token token = Token("blah", 0, 4, TokenType::string_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);

        const Token token2 = Token("=", 0, 1, TokenType::equal);
        const FileInfo fileInfo2 = FileInfo("test.swizzle");

        const TokenInfo equal = TokenInfo(token2, fileInfo2);
    };

    TEST_FIXTURE(WhenNextTokenIsAttributeValueAsStringLiteral, verifyConsume)
    {
        auto parserState = state.consume(equal, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(1U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructStartScope, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        REQUIRE CHECK(utils::nodeStackTopIs<nodes::Attribute>(attributeStack));

        const auto attributeNode = attributeStack.top();
        REQUIRE CHECK(attributeNode);

        auto attributeValueMatcher = Matcher().getChildrenOf<nodes::StringLiteral>().bind("value");
        REQUIRE CHECK(attributeValueMatcher(attributeNode));

        const auto valueNode = attributeValueMatcher.bound("value_0");
        REQUIRE CHECK(valueNode);

        const auto& value = static_cast<nodes::StringLiteral&>(*valueNode);
        CHECK_EQUAL("blah", value.info().token().value());
    }

    struct WhenNextTokenIsAttributeValueAsStringLiteralButTopOfNodeStackIsNotAttribute : public StructStartScopeStateFixture
    {
        const Token token = Token("blah", 0, 4, TokenType::string_literal);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsAttributeValueAsStringLiteralButTopOfNodeStackIsNotAttribute, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::ParserError);
    }

    struct WhenNextTokenIsAttributeBlock : public StructStartScopeStateFixture
    {
        WhenNextTokenIsAttributeBlock()
        {
            const Token t = Token("@attribute", 0, 10, TokenType::attribute);
            const FileInfo f = FileInfo("test.swizzle");

            attributeStack.push(new nodes::Attribute(TokenInfo(t, f)));
        }

        const Token token = Token("{size() != 0}", 0, 13, TokenType::attribute_block);
        const FileInfo fileInfo = FileInfo("test.swizzle");
        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsAttributeBlock, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(1U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructStartScope, parserState);

        REQUIRE CHECK_EQUAL(2U, nodeStack.size());
        REQUIRE CHECK_EQUAL(1U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        REQUIRE CHECK(utils::nodeStackTopIs<nodes::Attribute>(attributeStack));
        const auto attributeNode = attributeStack.top();
        REQUIRE CHECK(attributeNode);

        auto attributeValueMatcher = Matcher().getChildrenOf<nodes::AttributeBlock>().bind("value");
        REQUIRE CHECK(attributeValueMatcher(attributeNode));
        const auto valueNode = attributeValueMatcher.bound("value_0");
        REQUIRE CHECK(valueNode);

        const auto& value = static_cast<nodes::AttributeBlock&>(*valueNode);
        CHECK_EQUAL("{size() != 0}", value.info().token().value());
    }

    struct WhenNextTokenIsAttributeBlockButTopOfStackIsNotAttribute : public StructStartScopeStateFixture
    {
        const Token token = Token("{size() != 0}", 0, 13, TokenType::attribute_block);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsAttributeBlockButTopOfStackIsNotAttribute, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::ParserError);
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());

        CHECK_EQUAL("u8", tokenStack.top().token().value());
        CHECK(utils::nodeStackTopIs<nodes::StructField>(nodeStack));
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());

        CHECK_EQUAL("i8", tokenStack.top().token().value());
        CHECK(utils::nodeStackTopIs<nodes::StructField>(nodeStack));
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());

        CHECK_EQUAL("u16", tokenStack.top().token().value());
        CHECK(utils::nodeStackTopIs<nodes::StructField>(nodeStack));
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());

        CHECK_EQUAL("i16", tokenStack.top().token().value());
        CHECK(utils::nodeStackTopIs<nodes::StructField>(nodeStack));
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());

        CHECK_EQUAL("u32", tokenStack.top().token().value());
        CHECK(utils::nodeStackTopIs<nodes::StructField>(nodeStack));
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());

        CHECK_EQUAL("i32", tokenStack.top().token().value());
        CHECK(utils::nodeStackTopIs<nodes::StructField>(nodeStack));
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());

        CHECK_EQUAL("u64", tokenStack.top().token().value());
        CHECK(utils::nodeStackTopIs<nodes::StructField>(nodeStack));
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());

        CHECK_EQUAL("i64", tokenStack.top().token().value());
        CHECK(utils::nodeStackTopIs<nodes::StructField>(nodeStack));
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());

        CHECK_EQUAL("f32", tokenStack.top().token().value());
        CHECK(utils::nodeStackTopIs<nodes::StructField>(nodeStack));
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());

        CHECK_EQUAL("f64", tokenStack.top().token().value());
        CHECK(utils::nodeStackTopIs<nodes::StructField>(nodeStack));
    }

    struct WhenNextTokenIsBitfield : public StructStartScopeStateFixture
    {
        const Token token = Token("bitfield", 0, 8, TokenType::type);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsBitfield, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }

    struct WhenNextTokenIsVariableBlock : public StructStartScopeStateFixture
    {
        const Token token = Token("variable_block", 0, 14, TokenType::type);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsVariableBlock, verifyConsume)
    {
        CHECK_EQUAL(2U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructStartVariableBlock, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());
        CHECK(utils::nodeStackTopIs<nodes::VariableBlock>(nodeStack));
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());
        CHECK(utils::nodeStackTopIs<nodes::StructField>(nodeStack));
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
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldNamespaceOrType, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(1U, tokenStack.size());
        CHECK(utils::nodeStackTopIs<nodes::StructField>(nodeStack));
    }

    struct WhenNextTokenIsMemberName : public StructStartScopeStateFixture
    {
        WhenNextTokenIsMemberName()
        {
            const auto node = utils::appendNode<nodes::StructField>(nodeStack);
            nodeStack.push(node);
        }

        const Token token = Token("field1", 0, 6, TokenType::string);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsMemberName, verifyConsume)
    {
        CHECK_EQUAL(3U, nodeStack.size());
        CHECK_EQUAL(0U, attributeStack.size());
        CHECK_EQUAL(0U, tokenStack.size());

        const auto parserState = state.consume(info, nodeStack, attributeStack, tokenStack, context);

        CHECK_EQUAL(ParserState::StructFieldName, parserState);

        REQUIRE CHECK_EQUAL(3U, nodeStack.size());
        REQUIRE CHECK_EQUAL(0U, attributeStack.size());
        REQUIRE CHECK_EQUAL(0U, tokenStack.size());

        CHECK(utils::nodeStackTopIs<nodes::StructField>(nodeStack));
        const auto& structField = static_cast<nodes::StructField&>(*nodeStack.top());
        CHECK_EQUAL("field1", structField.name().token().value());
    }

    struct WhenNextTokenIsInvalid : public StructStartScopeStateFixture
    {
        const Token token = Token(";", 0, 1, TokenType::end_statement);
        const FileInfo fileInfo = FileInfo("test.swizzle");

        const TokenInfo info = TokenInfo(token, fileInfo);
    };

    TEST_FIXTURE(WhenNextTokenIsInvalid, verifyConsume)
    {
        CHECK_THROW(state.consume(info, nodeStack, attributeStack, tokenStack, context), swizzle::SyntaxError);
    }
}
