#pragma once 
#include <swizzle/lexer/TokenInfo.hpp>
#include <stack>

namespace swizzle { namespace parser {

    using TokenStack = std::stack<lexer::TokenInfo>;
}}
