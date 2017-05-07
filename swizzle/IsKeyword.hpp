#pragma once 
#include <boost/utility/string_view.hpp>

namespace swizzle {

    bool IsKeyword(const boost::string_view& token);
}
