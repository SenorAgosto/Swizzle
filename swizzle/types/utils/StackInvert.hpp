#pragma once 
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace types { namespace utils {

    // invert stack (make the bottom the top), leaves @stack empty.
    TokenStack invert(TokenStack& stack);
}}}
