#include <swizzle/parser/states/StartUsingState.hpp>

#include <swizzle/ast/nodes/TypeAlias.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/utils/AppendNode.hpp>
#include <swizzle/types/utils/AttachAttributes.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StartUsingState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack& attributeStack, types::TokenStack& tokenStack, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::string)
        {
            if(tokenStack.empty())
            {
                throw ParserError("Token Stack unexpectedly empty.");
            }

            const auto& info = tokenStack.top();
            const auto node = types::utils::appendNode<ast::nodes::TypeAlias>(nodeStack, info, token);
            
            const auto& alias = static_cast<ast::nodes::TypeAlias&>(*node);
            const auto aliasWithNamespace = context.CurrentNamespace + "::" + alias.aliasedType().token().value().to_string();
            
            context.SymbolTable.insert(aliasWithNamespace, types::SymbolInfo(aliasWithNamespace, types::SymbolType::TypeAlias, node));
            
            types::utils::attachAttributes(attributeStack, node);
            
            nodeStack.push(node);
            tokenStack.pop();

            return ParserState::UsingName;
        }

        throw SyntaxError("Expected string token", token);
    }
}}}
