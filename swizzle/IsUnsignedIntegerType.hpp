#pragma once 
#include <boost/utility/string_view.hpp>

namespace swizzle {

    bool IsUnsignedIntegerType(const boost::string_view& token);
}
