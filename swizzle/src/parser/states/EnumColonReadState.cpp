#include <swizzle/parser/states/EnumColonReadState.hpp>

#include <swizzle/ast/nodes/Enum.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/types/IsIntegerType.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState EnumColonReadState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack&, types::TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();
        const auto& value = token.token().value();

        if(type == lexer::TokenType::type)
        {
            if(!types::IsIntegerType(token.token().value()))
            {
                throw SyntaxError("Underlying type must be an integer type", token);
            }

            if(types::utils::nodeStackTopIs<ast::nodes::Enum>(nodeStack))
            {
                auto& Enump = static_cast<ast::nodes::Enum&>(*nodeStack.top());
                Enump.underlying(token);
                
                return ParserState::EnumUnderlyingType;
            }

            throw ParserError("Internal parser error: expected top of node stack to be ast::nodes::Enum");
        }

        throw SyntaxError("Expecting enum underlying type", token);
    }
}}}
