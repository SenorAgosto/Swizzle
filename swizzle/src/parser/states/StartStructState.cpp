#include <swizzle/parser/states/StartStructState.hpp>

#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StartStructState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, NodeStack&, TokenStack& tokenStack, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::string)
        {
            if(tokenStack.empty())
            {
                throw ParserError("Internal parser error, token stack empty. Expected struct keyword token info.");
            }

            auto& structKeyword = tokenStack.top();
            auto node = detail::appendNode<ast::nodes::Struct>(nodeStack, structKeyword, token, context.CurrentNamespace);

            nodeStack.push(node);
            tokenStack.pop();

            return ParserState::StructName;
        }

        throw SyntaxError("Expected struct name", token);
    }
}}}
