#pragma once 
#include <boost/utility/string_view.hpp>

namespace swizzle { namespace types {

    bool IsIntegerType(const boost::string_view& token);
}}
