#include <swizzle/parser/states/EnumStartScopeState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/Matcher.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/Enum.hpp>
#include <swizzle/ast/nodes/EnumField.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState EnumStartScopeState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack&, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::comment)
        {
            detail::appendNode<ast::nodes::Comment>(nodeStack, token);
            return ParserState::EnumStartScope;
        }

        if(type == lexer::TokenType::multiline_comment)
        {
            detail::appendNode<ast::nodes::MultilineComment>(nodeStack, token);
            return ParserState::EnumStartScope;
        }

        if(type == lexer::TokenType::string)
        {
            if(detail::nodeStackTopIs<ast::nodes::Enum>(nodeStack))
            {
                auto& top = static_cast<ast::nodes::Enum&>(*nodeStack.top());
                const auto node = detail::appendNode<ast::nodes::EnumField>(nodeStack, token, top.underlying());
                nodeStack.push(node);

                return ParserState::EnumField;
            }

            throw ParserError("Internal parser error, top of stack was not ast::nodes::Enum");
        }

        if(type == lexer::TokenType::r_brace)
        {
            if(detail::nodeStackTopIs<ast::nodes::Enum>(nodeStack))
            {
                auto hasNonCommentChildren = ast::Matcher().hasChildNotOf<ast::nodes::Comment, ast::nodes::MultilineComment>();
                if(!hasNonCommentChildren(nodeStack.top()))
                {
                    auto& top = static_cast<ast::nodes::Enum&>(*nodeStack.top());
                    throw SyntaxError("Enum must have fields", "no fields declared in '" + top.name() + "'", token.fileInfo());
                }

                context.ClearEnumValueAllocations();
                nodeStack.pop();

                return ParserState::TranslationUnitMain;
            }

            throw ParserError("Internal parser error, top of stack was not ast::nodes::Enum");
        }

        throw SyntaxError("Expected enum field definition", token);
    }
}}}
