#pragma once
#include <boost/utility/string_view.hpp>
#include <cstdint>

namespace swizzle { namespace types {

    struct isHexTag {} static const isHex = isHexTag();

    std::uint64_t setValue(const boost::string_view& underlying, const boost::string_view& value, const std::string& errorMessage);
    std::uint64_t setValue(const boost::string_view& underlying, const boost::string_view& value, const isHexTag&, const std::string& errorMessage);
}}
