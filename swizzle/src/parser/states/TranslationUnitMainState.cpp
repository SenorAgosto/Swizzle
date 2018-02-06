#include <swizzle/parser/states/TranslationUnitMainState.hpp>

#include <swizzle/ast/nodes/Attribute.hpp>
#include <swizzle/ast/nodes/AttributeBlock.hpp>
#include <swizzle/ast/nodes/CharLiteral.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/HexLiteral.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/ast/nodes/NumericLiteral.hpp>
#include <swizzle/ast/nodes/StringLiteral.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/utils/AppendNode.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>
#include <swizzle/types/TokenStack.hpp>

#include <limits>

namespace swizzle { namespace parser { namespace states {

    ParserState TranslationUnitMainState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack& attributeStack, types::TokenStack& tokenStack, ParserStateContext&)
    {
        const auto type = token.token().type();

        // we only push ast::nodes::Attribute onto the attribute stack,
        // so we know if the stack is non-empty we have an attribute
        if(!attributeStack.empty())
        {
            if(type == lexer::TokenType::equal)
            {
                return ParserState::TranslationUnitMain;
            }

            if(type == lexer::TokenType::char_literal)
            {
                types::utils::appendNode<ast::nodes::CharLiteral>(attributeStack, token);
                return ParserState::TranslationUnitMain;
            }

            if(type == lexer::TokenType::string_literal)
            {
                types::utils::appendNode<ast::nodes::StringLiteral>(attributeStack, token);
                return ParserState::TranslationUnitMain;
            }

            if(type == lexer::TokenType::hex_literal)
            {
                types::utils::appendNode<ast::nodes::HexLiteral>(attributeStack, token);
                return ParserState::TranslationUnitMain;
            }

            if(type == lexer::TokenType::numeric_literal)
            {
                types::utils::appendNode<ast::nodes::NumericLiteral>(attributeStack, token);
                return ParserState::TranslationUnitMain;
            }

            if(type == lexer::TokenType::attribute_block)
            {
                types::utils::appendNode<ast::nodes::AttributeBlock>(attributeStack, token);
                return ParserState::TranslationUnitMain;
            }
        }

        if(type == lexer::TokenType::attribute)
        {
            attributeStack.push(new ast::nodes::Attribute(token));
            return ParserState::TranslationUnitMain;
        }

        if(type == lexer::TokenType::comment)
        {
            types::utils::appendNode<ast::nodes::Comment>(nodeStack, token);
            return ParserState::TranslationUnitMain;
        }

        if(type == lexer::TokenType::multiline_comment)
        {
            types::utils::appendNode<ast::nodes::MultilineComment>(nodeStack, token);
            return ParserState::TranslationUnitMain;
        }

        const auto& value = token.token().value();
        if((type == lexer::TokenType::type) && (value == "bitfield"))
        {
            tokenStack.push(token);
            return ParserState::StartBitfield;
        }

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
                return ParserState::StartEnum;
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
