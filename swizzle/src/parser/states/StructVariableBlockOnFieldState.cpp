#include <swizzle/parser/states/StructVariableBlockOnFieldState.hpp>

#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructVariableBlockOnFieldState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack& tokenStack)
    {
        // TODO: implement
        return ParserState::Init;
    }
}}}
