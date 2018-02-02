#include <swizzle/parser/states/StructFieldEnumOrNamespaceReadState.hpp>
#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/ast/nodes/DefaultValue.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/types/utils/AppendNode.hpp>
#include <swizzle/types/utils/CreateType.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>
#include <swizzle/types/utils/ValidateEnumValue.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructFieldEnumOrNamespaceReadState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack&, types::TokenStack& tokenStack, ParserStateContext& context)
    {
        const auto type = token.token().type();
        
        if(type == lexer::TokenType::colon)
        {
            tokenStack.push(token);
            return ParserState::StructFieldEnumOrNamespaceFirstColonRead;
        }
        
        if(type == lexer::TokenType::end_statement)
        {
            if(types::utils::nodeStackTopIs<ast::nodes::StructField>(nodeStack))
            {
                const auto& structField = static_cast<ast::nodes::StructField&>(*nodeStack.top());
                if(types::utils::is_enum(structField.type()))
                {
                    const auto enumValue = types::utils::createType(tokenStack);
                    
                    if(types::utils::validateEnumValue(context, enumValue))
                    {
                        types::utils::appendNode<ast::nodes::DefaultValue>(nodeStack, token, structField.type());
                        return ParserState::StructStartScope;
                    }
        
                    throw SyntaxError("Expected enum value", token);
                }
                
                throw SyntaxError("Attempting to assign enum value to non-enum field", token);
            }
            
            throw ParserError("Expected top of node stack to be ast::nodes::StructField but wasn't");
        }
        
        throw SyntaxError("Expected ':' or ';'", token);
    }
}}}
