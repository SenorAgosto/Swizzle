#include <swizzle/parser/states/StructBaseNameOrNamespaceReadState.hpp>

#include <swizzle/ast/nodes/BaseClass.hpp>
#include <swizzle/ast/nodes/Struct.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/utils/AppendNode.hpp>
#include <swizzle/types/utils/AttachAttributes.hpp>
#include <swizzle/types/utils/CreateType.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructBaseNameOrNamespaceReadState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack& attributeStack, types::TokenStack& tokenStack, ParserStateContext& context)
    {
        const auto type = token.token().type();
        
        if(type == lexer::TokenType::colon)
        {
            return ParserState::StructBaseNameOrNamespaceFirstColonRead;
        }
        
        if(type == lexer::TokenType::l_brace)
        {
            const auto base_class = types::utils::createType(tokenStack);
            const auto info = context.SymbolTable.find(context.CurrentNamespace, base_class.token().to_string(), SyntaxError("Base class type not defined", token));
            
            if(types::utils::nodeStackTopIs<ast::nodes::Struct>(nodeStack))
            {
                auto node = types::utils::appendNode<ast::nodes::BaseClass>(nodeStack, static_cast<ast::nodes::Struct&>(*info.node()));
                types::utils::attachAttributes(attributeStack, node);
                
                return ParserState::StructStartScope;
            }
    
            throw ParserError("Internal parser error, expected top of node stack to be ast::nodes::Struct");
        }

        if(type == lexer::TokenType::comma)
        {
            const auto base_class = types::utils::createType(tokenStack);
            const auto info = context.SymbolTable.find(context.CurrentNamespace, base_class.token().to_string(), SyntaxError("Base class type not defined", token));
            
            if(types::utils::nodeStackTopIs<ast::nodes::Struct>(nodeStack))
            {
                auto node = types::utils::appendNode<ast::nodes::BaseClass>(nodeStack, static_cast<ast::nodes::Struct&>(*info.node()));
                types::utils::attachAttributes(attributeStack, node);
                
                return ParserState::StructBaseColonRead;
            }
            
            throw ParserError("Internal parser error, expected top of node stack to be ast::nodes::Struct");
        }

        throw SyntaxError("Expected ':'", token);
    }
}}}



