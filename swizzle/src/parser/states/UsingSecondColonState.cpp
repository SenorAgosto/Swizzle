#include <swizzle/parser/states/UsingSecondColonState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState UsingSecondColonState::consume(const lexer::TokenInfo& token, NodeStack&, NodeStack&, TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::colon)
        {
            return ParserState::UsingTypeRead;
        }

        throw SyntaxError("Expected ':'", token);
    }
}}}
