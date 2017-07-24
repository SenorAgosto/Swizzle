#pragma once
#include <swizzle/Exceptions.hpp>
#include <swizzle/types/SafeStringStream.hpp>

#include <boost/utility/string_view.hpp>

namespace swizzle { namespace types {

    template<class T>
    T readAs(const boost::string_view& value)
    {
        safe_istringstream is(value);

        T t = 0;
        is >> t;

        return t;
    }

    template<class T>
    T readAsHex(const boost::string_view& value)
    {
        safe_istringstream is(value);

        T t = 0;
        is >> hex_stream >> t;

        return t;
    }
}}
