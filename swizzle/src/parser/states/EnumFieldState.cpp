#include <swizzle/parser/states/EnumFieldState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/Enum.hpp>
#include <swizzle/ast/nodes/EnumField.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState EnumFieldState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack&, types::TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::equal)
        {
            return ParserState::EnumFieldEqualRead;
        }

        if(type == lexer::TokenType::comma)
        {
            if(types::utils::nodeStackTopIs<ast::nodes::EnumField>(nodeStack))
            {
                try
                {
                    const auto field_node = nodeStack.top();
                    auto& field = static_cast<ast::nodes::EnumField&>(*field_node);
                    
                    nodeStack.pop();

                    if(types::utils::nodeStackTopIs<ast::nodes::Enum>(nodeStack))
                    {
                        auto& Enum = static_cast<ast::nodes::Enum&>(*nodeStack.top());
                        Enum.assign_enum_field_value(field, token);
                        
                        return ParserState::EnumStartScope;
                    }
                    
                    throw ParserError("Expected top of node stack to be Enum");
                    
                }
                catch(const std::out_of_range&)
                {
                    throw SyntaxError("Enum value overflowed underlying type", token);
                }
            }

            throw ParserError("Internal parser error, top of node stack is not ast::nodes::EnumField");
        }

        throw SyntaxError("Expected ',' or '='", token);
    }
}}}
