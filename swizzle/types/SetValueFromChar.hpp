#pragma once 
#include <boost/utility/string_view.hpp>


namespace swizzle { namespace types {

    std::uint64_t setValueFromChar(const boost::string_view& underlying, const boost::string_view& value);

}}
