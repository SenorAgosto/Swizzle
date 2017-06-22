#pragma once 
#include <swizzle/parser/TokenList.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace utils { namespace stack {

    // consumes @stack creating a TokenList
    TokenList to_list(TokenStack& stack);
}}}}
