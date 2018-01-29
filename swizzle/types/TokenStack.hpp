#pragma once 
#include <swizzle/lexer/TokenInfo.hpp>
#include <stack>

namespace swizzle { namespace types {

    using TokenStack = std::stack<lexer::TokenInfo>;
}}
