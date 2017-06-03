#include <swizzle/parser/states/TranslationUnitMainState.hpp>

#include <swizzle/Exceptions.hpp>

#include <swizzle/ast/nodes/Attribute.hpp>
#include <swizzle/ast/nodes/AttributeBlock.hpp>
#include <swizzle/ast/nodes/CharLiteral.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/HexLiteral.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/ast/nodes/NumericLiteral.hpp>
#include <swizzle/ast/nodes/StringLiteral.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState TranslationUnitMainState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack& tokenStack, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::char_literal)
        {
            detail::appendNode<ast::nodes::CharLiteral>(nodeStack, token);
            nodeStack.pop();

            return ParserState::TranslationUnitMain;
        }

        if(type == lexer::TokenType::string_literal)
        {
            detail::appendNode<ast::nodes::StringLiteral>(nodeStack, token);
            nodeStack.pop();

            return ParserState::TranslationUnitMain;
        }

        if(type == lexer::TokenType::hex_literal)
        {
            detail::appendNode<ast::nodes::HexLiteral>(nodeStack, token);
            nodeStack.pop();

            return ParserState::TranslationUnitMain;
        }

        if(type == lexer::TokenType::numeric_literal)
        {
            detail::appendNode<ast::nodes::NumericLiteral>(nodeStack, token);
            nodeStack.pop();

            return ParserState::TranslationUnitMain;
        }

        if(type == lexer::TokenType::attribute_block)
        {
            detail::appendNode<ast::nodes::AttributeBlock>(nodeStack, token);
            nodeStack.pop();

            return ParserState::TranslationUnitMain;
        }

        if(type == lexer::TokenType::attribute)
        {
            auto attribute = detail::appendNode<ast::nodes::Attribute>(nodeStack, token);
            nodeStack.push(attribute);

            return ParserState::TranslationUnitMain;
        }

        // cleanup the node stack
        if(detail::nodeStackTopIs<ast::nodes::Attribute>(nodeStack))
        {
            nodeStack.pop();
        }

        if(type == lexer::TokenType::comment)
        {
            detail::appendNode<ast::nodes::Comment>(nodeStack, token);
            return ParserState::TranslationUnitMain;
        }

        if(type == lexer::TokenType::multiline_comment)
        {
            detail::appendNode<ast::nodes::MultilineComment>(nodeStack, token);
            return ParserState::TranslationUnitMain;
        }

        const auto& value = token.token().value();
        if(type == lexer::TokenType::keyword)
        {
            if(value == "using")
            {
                tokenStack.push(token);
                return ParserState::StartUsing;
            }

            if(value == "enum")
            {
                tokenStack.push(token);
                context.CurrentEnumValue = types::EnumValue();

                return ParserState::StartEnum;
            }

            if(value == "bitfield")
            {
                tokenStack.push(token);
                context.CurrentBitfieldBit = 0;
                
                return ParserState::StartBitfield;
            }

            if(value == "struct")
            {
                tokenStack.push(token);
                return ParserState::StartStruct;
            }
        }

        throw SyntaxError("Unexpected token type encountered", token);
    }
}}}
