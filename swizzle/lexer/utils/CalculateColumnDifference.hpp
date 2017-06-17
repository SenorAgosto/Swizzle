#pragma once
#include <cstddef>

namespace swizzle { namespace lexer {
    class TokenInfo;
}}

namespace swizzle { namespace lexer { namespace utils {

    // return difference between end of @top and end of @info.
    std::size_t calculateColumnDifference(const TokenInfo& info, const TokenInfo& top);
}}}
