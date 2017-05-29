#pragma once
#include <swizzle/parser/TokenStack.hpp>
#include <string>

namespace swizzle { namespace parser { namespace detail {

    lexer::TokenInfo createType(TokenStack& tokenStack);
}}}
