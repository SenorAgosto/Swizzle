#include <swizzle/parser/states/EnumColonReadState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/Enum.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>
#include <swizzle/types/IsIntegerType.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState EnumColonReadState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();
        const auto& value = token.token().value();

        if(type == lexer::TokenType::type)
        {
            if(!types::IsIntegerType(token.token().value()))
            {
                throw SyntaxError("Underlying type must be an integer type", token);
            }

            if(detail::nodeStackTopIs<ast::nodes::Enum>(nodeStack))
            {
                auto& top = static_cast<ast::nodes::Enum&>(*nodeStack.top());
                top.underlying(token);
            }
            else
            {
                throw ParserError("Internal parser error: expected top of node stack to be ast::nodes::Enum");
            }

            return ParserState::EnumUnderlyingType;
        }

        throw SyntaxError("Expecting enum underlying type", token);
    }
}}}
