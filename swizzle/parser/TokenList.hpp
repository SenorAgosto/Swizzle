#pragma once 
#include <swizzle/lexer/TokenInfo.hpp>
#include <deque>

namespace swizzle { namespace parser {

    using TokenList = std::deque<lexer::TokenInfo>;
}}
