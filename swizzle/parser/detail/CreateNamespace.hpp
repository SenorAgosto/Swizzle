#pragma once
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace detail {

    lexer::TokenInfo createNamespace(TokenStack& tokenStack);
}}}
