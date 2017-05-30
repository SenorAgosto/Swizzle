#include <swizzle/parser/states/EnumUnderlyingTypeState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState EnumUnderlyingTypeState::consume(const lexer::TokenInfo& token, NodeStack&, TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::l_brace)
        {
            return ParserState::EnumStartScope;
        }

        throw SyntaxError("Expected '{'", token);
    }
}}}
