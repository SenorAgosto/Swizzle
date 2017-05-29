#include <swizzle/parser/states/UsingSecondColonState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

#include <sstream>

namespace swizzle { namespace parser { namespace states {

    ParserState UsingSecondColonState::consume(const lexer::TokenInfo& token, NodeStack&, TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::colon)
        {
            return ParserState::UsingTypeRead;
        }

        std::stringstream ss;
        ss << type;

        throw SyntaxError("Expected ':', found " + ss.str() + " (" + token.token().to_string() + ")", token);
    }
}}}
