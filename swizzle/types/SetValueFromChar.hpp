#pragma once 
#include <swizzle/types/EnumValueType.hpp>
#include <boost/utility/string_view.hpp>

namespace swizzle { namespace types {

    EnumValueType setValueFromChar(const boost::string_view& underlying, const boost::string_view& value);

}}
