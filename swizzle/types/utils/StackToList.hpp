#pragma once
#include <swizzle/types/TokenList.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace types { namespace utils {

    // consumes @stack creating a TokenList
    TokenList to_list(TokenStack& stack);
}}}
