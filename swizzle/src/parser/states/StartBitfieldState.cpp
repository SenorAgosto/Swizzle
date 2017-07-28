#include <swizzle/parser/states/StartBitfieldState.hpp>

#include <swizzle/ast/nodes/Bitfield.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/detail/AttachAttributes.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StartBitfieldState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, NodeStack& attributeStack, TokenStack& tokenStack, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::string)
        {
            if(tokenStack.empty())
            {
                throw ParserError("Token Stack unexpectedly empty.");
            }

            const auto& info = tokenStack.top();
            const auto node = detail::appendNode<ast::nodes::Bitfield>(nodeStack, info, token, context.CurrentNamespace);

            detail::attachAttributes(attributeStack, node);

            nodeStack.push(node);
            tokenStack.pop();
            
            return ParserState::BitfieldName;
        }

        throw SyntaxError("Expected bitfield name", token);
    }
}}}
