#pragma once 
#include <swizzle/lexer/TokenInfo.hpp>
#include <deque>

namespace swizzle { namespace parser {

    using TokenStack = std::deque<lexer::TokenInfo>;
}}
