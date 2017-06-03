#include <swizzle/parser/states/BitfieldStartScopeState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/Bitfield.hpp>
#include <swizzle/ast/nodes/BitfieldField.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState BitfieldStartScopeState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::comment)
        {
            detail::appendNode<ast::nodes::Comment>(nodeStack, token);
            return ParserState::BitfieldStartScope;
        }

        if(type == lexer::TokenType::multiline_comment)
        {
            detail::appendNode<ast::nodes::MultilineComment>(nodeStack, token);
            return ParserState::BitfieldStartScope;
        }

        if(type == lexer::TokenType::string)
        {
            if(detail::nodeStackTopIs<ast::nodes::Bitfield>(nodeStack))
            {
                auto& top = static_cast<ast::nodes::Bitfield&>(*nodeStack.top());
                const auto node = detail::appendNode<ast::nodes::BitfieldField>(nodeStack, token, top.underlying());
                nodeStack.push(node);

                return ParserState::BitfieldField;
            }

            throw ParserError("Internal parser error, top of stack was not ast::nodes::Bitfield");
        }

        if(type == lexer::TokenType::r_brace)
        {
            if(detail::nodeStackTopIs<ast::nodes::Bitfield>(nodeStack))
            {
                auto& top = static_cast<ast::nodes::Bitfield&>(*nodeStack.top());
                if(top.empty())
                {
                    throw SyntaxError("Enum must have fields, no fields declared in '" + top.name() + "'", token);
                }

                nodeStack.pop();

                return ParserState::TranslationUnitMain;
            }

            throw ParserError("Internal parser error, top of stack was not ast::nodes::Bitfield");
        }

        throw SyntaxError("Expected bitfield field definition", token);
    }
}}}
