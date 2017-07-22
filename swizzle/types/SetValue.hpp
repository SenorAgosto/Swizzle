#pragma once
#include <boost/utility/string_view.hpp>
#include <swizzle/types/EnumValueType.hpp>

namespace swizzle { namespace types {

    struct isHexTag {} static const isHex = isHexTag();

    EnumValueType setValue(const boost::string_view& underlying, const boost::string_view& value);
    EnumValueType setValue(const boost::string_view& underlying, const boost::string_view& value, const isHexTag&);
}}
