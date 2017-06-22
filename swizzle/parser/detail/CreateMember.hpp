#pragma once 
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser {
    struct ParserStateContext;
}}

namespace swizzle { namespace parser { namespace detail {

    lexer::TokenInfo createMember(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack& tokenStack, const ParserStateContext& context);

}}}
