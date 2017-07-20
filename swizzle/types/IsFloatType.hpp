#pragma once 
#include <boost/utility/string_view.hpp>

namespace swizzle { namespace types {

    bool IsFloatType(const boost::string_view& token);
}}
