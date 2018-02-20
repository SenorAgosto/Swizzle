#include <swizzle/parser/states/StartStructState.hpp>

#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/utils/AppendNode.hpp>
#include <swizzle/types/utils/AttachAttributes.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StartStructState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack& attributeStack, types::TokenStack& tokenStack, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::string)
        {
            if(tokenStack.empty())
            {
                throw ParserError("Internal parser error, token stack empty. Expected struct keyword token info.");
            }

            auto& structKeyword = tokenStack.top();
            auto node = types::utils::appendNode<ast::nodes::Struct>(nodeStack, structKeyword, token, context.CurrentNamespace);

            types::utils::attachAttributes(attributeStack, node);

            const auto structNode = static_cast<ast::nodes::Struct&>(*node);
            if(context.SymbolTable.contains(context.CurrentNamespace, structNode.name()))
            {
                throw RedefinitionOfStructTypeException(structNode.name(), structNode.keyword().fileInfo());
            }
            
            context.SymbolTable.insert(structNode.name(), types::SymbolInfo(structNode.name(), types::SymbolType::Struct, node));

            nodeStack.push(node);
            tokenStack.pop();

            return ParserState::StructName;
        }

        throw SyntaxError("Expected struct name", token);
    }
}}}
