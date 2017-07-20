#pragma once 
#include <boost/utility/string_view.hpp>

namespace swizzle { namespace types {

    bool IsUnsignedIntegerType(const boost::string_view& token);
}}
