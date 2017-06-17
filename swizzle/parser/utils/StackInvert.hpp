#pragma once 
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace utils { namespace stack {

    // invert stack (make the bottom the top), leaves @stack empty.
    TokenStack invert(TokenStack& stack);
}}}}
