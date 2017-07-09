#pragma once
#include <cstddef>
#include <boost/utility/string_view.hpp>

namespace swizzle { namespace lexer {
    class TokenInfo;
}}

namespace swizzle { namespace parser { namespace detail {

    // @return the numeric value stored in @info, perform bounds checking against @underlying raise an exception
    // if the value in @info is out-of-bounds
    std::size_t extractBitValueFromToken(const boost::string_view& underlying, const lexer::TokenInfo& info);
}}}
