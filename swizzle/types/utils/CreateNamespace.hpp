#pragma once
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace types { namespace utils {

    lexer::TokenInfo createNamespace(TokenStack& tokenStack);
}}}
