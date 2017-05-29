#include <swizzle/parser/states/StructStartArrayState.hpp>

#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructStartArrayState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack& tokenStack, ParserStateContext& context)
    {
        // TODO: implement
        return ParserState::Init;
    }
}}}
