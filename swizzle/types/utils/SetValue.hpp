#pragma once
#include <boost/utility/string_view.hpp>
#include <cstdint>

namespace swizzle { namespace lexer {
    class TokenInfo;
}}

namespace swizzle { namespace types {

    struct isHexTag {} static const isHex = isHexTag();

    std::uint64_t setValue(const boost::string_view& underlying, const lexer::TokenInfo& token, const std::string& errorMessage);
    std::uint64_t setValue(const boost::string_view& underlying, const lexer::TokenInfo& token, const isHexTag&, const std::string& errorMessage);
}}
