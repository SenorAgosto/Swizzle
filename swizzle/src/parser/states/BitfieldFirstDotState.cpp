#include <swizzle/parser/states/BitfieldFirstDotState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState BitfieldFirstDotState::consume(const lexer::TokenInfo& token, types::NodeStack&, types::NodeStack&, types::TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::dot)
        {
            return ParserState::BitfieldSecondDot;
        }

        throw SyntaxError("Expected '.'", token);
    }
}}}
