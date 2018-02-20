#include <swizzle/parser/states/StructBaseNameOrNamespaceReadState.hpp>

#include <swizzle/ast/nodes/BaseClass.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/utils/AppendNode.hpp>
#include <swizzle/types/utils/CreateType.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructBaseNameOrNamespaceReadState::consume(const lexer::TokenInfo& token, types::NodeStack&, types::NodeStack& nodeStack, types::TokenStack& tokenStack, ParserStateContext& context)
    {
        const auto type = token.token().type();
        
        if(type == lexer::TokenType::colon)
        {
            return ParserState::StructBaseNameOrNamespaceFirstColonRead;
        }
        
        if(type == lexer::TokenType::l_brace)
        {
            const auto base_class = types::utils::createType(tokenStack);
        
            if(context.SymbolTable.contains(context.CurrentNamespace, base_class.token().to_string()))
            {
                types::utils::appendNode<ast::nodes::BaseClass>(nodeStack, base_class);
                return ParserState::StructStartScope;
            }
        
            throw SyntaxError("Base class type not defined", token);
        }

        if(type == lexer::TokenType::comma)
        {
            const auto base_class = types::utils::createType(tokenStack);
            
            if(context.SymbolTable.contains(context.CurrentNamespace, base_class.token().to_string()))
            {
                types::utils::appendNode<ast::nodes::BaseClass>(nodeStack, base_class);
                return ParserState::StructBaseColonRead;
            }
            
            throw SyntaxError("Inheriting from undefined type", token);
        }

        throw SyntaxError("Expected ':'", token);
    }
}}}



