#include <swizzle/parser/states/StructBaseColonReadState.hpp>

#include <swizzle/ast/nodes/Attribute.hpp>
#include <swizzle/ast/nodes/AttributeBlock.hpp>
#include <swizzle/ast/nodes/BaseClass.hpp>
#include <swizzle/ast/nodes/CharLiteral.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/HexLiteral.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/ast/nodes/NumericLiteral.hpp>
#include <swizzle/ast/nodes/StringLiteral.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenType.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/utils/AppendNode.hpp>
#include <swizzle/types/utils/CreateType.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructBaseColonReadState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack& attributeStack, types::TokenStack& tokenStack, ParserStateContext&)
    {
        const auto type = token.token().type();
        
        if(equalRead_)
        {
            if(type == lexer::TokenType::equal)
            {
                throw SyntaxError("Expected literal value", token);
            }

            if(type == lexer::TokenType::numeric_literal)
            {
                equalRead_ = false;
                types::utils::appendNode<ast::nodes::NumericLiteral>(attributeStack, token);

                return ParserState::StructBaseColonRead;
            }

            if(type == lexer::TokenType::hex_literal)
            {
                equalRead_ = false;
                types::utils::appendNode<ast::nodes::HexLiteral>(attributeStack, token);

                return ParserState::StructBaseColonRead;
            }

            if(type == lexer::TokenType::char_literal)
            {
                equalRead_ = false;
                types::utils::appendNode<ast::nodes::CharLiteral>(attributeStack, token);

                return ParserState::StructBaseColonRead;
            }

            if(type == lexer::TokenType::string_literal)
            {
                equalRead_ = false;
                types::utils::appendNode<ast::nodes::StringLiteral>(attributeStack, token);

                return ParserState::StructBaseColonRead;
            }
        }
        else
        {
            if(type == lexer::TokenType::equal)
            {
                equalRead_ = true;
                return ParserState::StructBaseColonRead;
            }

            if(type == lexer::TokenType::attribute_block)
            {
                if(!attributeStack.empty())
                {
                    types::utils::appendNode<ast::nodes::AttributeBlock>(attributeStack, token);
                    return ParserState::StructBaseColonRead;
                }
            }

            if(type == lexer::TokenType::attribute)
            {
                attributeStack.push(new ast::nodes::Attribute(token));
                return ParserState::StructBaseColonRead;
            }

            if(type == lexer::TokenType::comment)
            {
                types::utils::appendNode<ast::nodes::Comment>(nodeStack, token);
                return ParserState::StructBaseColonRead;
            }

            if(type == lexer::TokenType::multiline_comment)
            {
                types::utils::appendNode<ast::nodes::MultilineComment>(nodeStack, token);
                return ParserState::StructBaseColonRead;
            }
            
            if(type == lexer::TokenType::string)
            {
                tokenStack.push(token);
                return ParserState::StructBaseNameOrNamespaceRead;
            }
        }
        
        throw SyntaxError("Expected struct name, namespace, comment, or attribute", token);
    }
}}}

