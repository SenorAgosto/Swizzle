#include <swizzle/parser/states/StartUsingState.hpp>

#include <swizzle/ast/nodes/TypeAlias.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StartUsingState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack& tokenStack, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::string)
        {
            if(tokenStack.empty())
            {
                throw ParserError("Token Stack unexpectedly empty.");
            }

            const auto& info = tokenStack.top();
            const auto node = detail::appendNode<ast::nodes::TypeAlias>(nodeStack, info, token);

            nodeStack.push(node);
            tokenStack.pop();

            return ParserState::UsingName;
        }

        throw SyntaxError("Expected string token", token);
    }
}}}
