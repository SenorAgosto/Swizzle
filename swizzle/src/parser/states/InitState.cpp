#include <swizzle/parser/states/InitState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/lexer/TokenType.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState InitState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::comment)
        {
            detail::appendNode<ast::nodes::Comment>(nodeStack, token);
            return ParserState::Init;
        }

        if(type == lexer::TokenType::multiline_comment)
        {
            detail::appendNode<ast::nodes::MultilineComment>(nodeStack, token);
            return ParserState::Init;
        }

        const auto& value = token.token().value();
        if(type == lexer::TokenType::keyword)
        {
            if("import" == value)
            {
                return ParserState::StartImport;
            }

            if("extern" == value)
            {
                return ParserState::StartExtern;
            }

            if("namespace" == value)
            {
                return ParserState::StartNamespace;
            }
        }

        throw SyntaxError("Expected comment, import statement, or namespace declaration", token);
    }
}}}
