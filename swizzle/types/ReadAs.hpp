#pragma once
#include <boost/utility/string_view.hpp>
#include <sstream>

namespace swizzle { namespace types {

    template<class T>
    T readAs(const boost::string_view& value)
    {
        std::istringstream is(value.data(), value.length());

        T t;
        is >> t;

        return t;
    }

}}
