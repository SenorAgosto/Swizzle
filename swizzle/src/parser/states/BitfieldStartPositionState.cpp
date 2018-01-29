#include <swizzle/parser/states/BitfieldStartPositionState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState BitfieldStartPositionState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack&, types::TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::comma)
        {
            nodeStack.pop();
            return ParserState::BitfieldStartScope;
        }

        if(type == lexer::TokenType::dot)
        {
            return ParserState::BitfieldFirstDot;
        }

        throw SyntaxError("Expected ',' or '.'", token);
    }
}}}
