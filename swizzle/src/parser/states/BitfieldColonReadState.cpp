#include <swizzle/parser/states/BitfieldColonReadState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/Bitfield.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>
#include <swizzle/types/IsUnsignedIntegerType.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState BitfieldColonReadState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, NodeStack&, TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::type)
        {
            if(!types::IsUnsignedIntegerType(token.token().value()))
            {
                throw SyntaxError("Underlying type must be unsigned integer type", token);
            }

            if(detail::nodeStackTopIs<ast::nodes::Bitfield>(nodeStack))
            {
                auto& top = static_cast<ast::nodes::Bitfield&>(*nodeStack.top());
                top.underlying(token);
            }
            else
            {
                throw ParserError("Internal parser error: expected top of node stack to be ast::nodes::Bitfield");
            }

            return ParserState::BitfieldUnderlyingType;

        }

        throw SyntaxError("Expected underlying type", token);
    }
}}}
