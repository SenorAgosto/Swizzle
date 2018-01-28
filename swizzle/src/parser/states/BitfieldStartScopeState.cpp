#include <swizzle/parser/states/BitfieldStartScopeState.hpp>

#include <swizzle/ast/nodes/Attribute.hpp>
#include <swizzle/ast/nodes/AttributeBlock.hpp>
#include <swizzle/ast/nodes/Bitfield.hpp>
#include <swizzle/ast/nodes/BitfieldField.hpp>
#include <swizzle/ast/nodes/CharLiteral.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/HexLiteral.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/ast/nodes/NumericLiteral.hpp>
#include <swizzle/ast/nodes/StringLiteral.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/utils/AppendNode.hpp>
#include <swizzle/types/utils/AttachAttributes.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState BitfieldStartScopeState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack& attributeStack, types::TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(!attributeStack.empty())
        {
            if(type == lexer::TokenType::equal)
            {
                return ParserState::BitfieldStartScope;
            }

            if(type == lexer::TokenType::char_literal)
            {
                types::utils::appendNode<ast::nodes::CharLiteral>(attributeStack, token);
                return ParserState::BitfieldStartScope;
            }

            if(type == lexer::TokenType::string_literal)
            {
                types::utils::appendNode<ast::nodes::StringLiteral>(attributeStack, token);
                return ParserState::BitfieldStartScope;
            }

            if(type == lexer::TokenType::hex_literal)
            {
                types::utils::appendNode<ast::nodes::HexLiteral>(attributeStack, token);
                return ParserState::BitfieldStartScope;
            }

            if(type == lexer::TokenType::numeric_literal)
            {
                types::utils::appendNode<ast::nodes::NumericLiteral>(attributeStack, token);
                return ParserState::BitfieldStartScope;
            }

            if(type == lexer::TokenType::attribute_block)
            {
                types::utils::appendNode<ast::nodes::AttributeBlock>(attributeStack, token);
                return ParserState::BitfieldStartScope;
            }
        }

        if(type == lexer::TokenType::attribute)
        {
            attributeStack.push(new ast::nodes::Attribute(token));
            return ParserState::BitfieldStartScope;
        }

        if(type == lexer::TokenType::comment)
        {
            types::utils::appendNode<ast::nodes::Comment>(nodeStack, token);
            return ParserState::BitfieldStartScope;
        }

        if(type == lexer::TokenType::multiline_comment)
        {
            types::utils::appendNode<ast::nodes::MultilineComment>(nodeStack, token);
            return ParserState::BitfieldStartScope;
        }

        if(type == lexer::TokenType::string)
        {
            if(types::utils::nodeStackTopIs<ast::nodes::Bitfield>(nodeStack))
            {
                auto& top = static_cast<ast::nodes::Bitfield&>(*nodeStack.top());
                const auto node = types::utils::appendNode<ast::nodes::BitfieldField>(nodeStack, token, top.underlying());

                types::utils::attachAttributes(attributeStack, node);
                nodeStack.push(node);

                return ParserState::BitfieldField;
            }

            throw ParserError("Internal parser error, top of stack was not ast::nodes::Bitfield");
        }

        if(type == lexer::TokenType::r_brace)
        {
            if(types::utils::nodeStackTopIs<ast::nodes::Bitfield>(nodeStack))
            {
                auto& top = static_cast<ast::nodes::Bitfield&>(*nodeStack.top());
                if(top.empty())
                {
                    throw SyntaxError("'bitfield' must have fields, no fields declared in '" + top.name() + "'", token);
                }

                nodeStack.pop();

                return ParserState::TranslationUnitMain;
            }

            throw ParserError("Internal parser error, top of stack was not ast::nodes::Bitfield");
        }

        throw SyntaxError("Expected bitfield field definition", token);
    }
}}}
