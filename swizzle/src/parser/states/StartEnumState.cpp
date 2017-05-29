#include <swizzle/parser/states/StartEnumState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/Enum.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

#include <sstream>

namespace swizzle { namespace parser { namespace states {

    ParserState StartEnumState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack& tokenStack, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::string)
        {
            if(tokenStack.empty())
            {
                throw ParserError("Token Stack unexpectedly empty.");
            }

            const auto& info = tokenStack.top();
            const auto node = detail::appendNode<ast::nodes::Enum>(nodeStack, info, token, context.CurrentNamespace);

            nodeStack.push(node);
            tokenStack.pop();
            
            return ParserState::EnumName;
        }

        std::stringstream ss;
        ss << type;

        throw SyntaxError("Expected enum name, found " + ss.str() + " (" + token.token().to_string() + ")", token);
    }
}}}
