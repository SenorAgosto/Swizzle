#pragma once
#include <swizzle/Exceptions.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/utility/string_view.hpp>
#include <cstdint>
#include <stdexcept>
#include <sstream>
#include <string>
#include <type_traits>

namespace swizzle { namespace types {

    template<class T>
    T readAs(const boost::string_view& value)
    {
        static_assert(!std::is_same<T, std::uint8_t>::value && !std::is_same<T, std::int8_t>::value, "Do not instantiate this function template with uint8_t or int8_t types.");

        std::istringstream is(std::string(value.data(), value.length()));

        T t = 0;
        is >> t;

        if(!is.eof())
        {
            throw StreamNotFullyConsumed(value);
        }

        return t;
    }

    template<class T>
    T readAsHex(const boost::string_view& value)
    {
        if(value.length() != (sizeof(T) * 2 + 2))
        {
            throw std::out_of_range("'value.length != " + boost::lexical_cast<std::string>(sizeof(T) + 2) + "' in readAsHex()");
        }

        // trim leading 0x
        std::istringstream is(std::string(value.data() + 2, value.length() - 2));

        T t = 0;
        is >> std::hex >> t;

        if(!is.eof())
        {
            throw StreamNotFullyConsumed(value);
        }

        return t;
    }

    std::uint8_t readAsU8(const boost::string_view& value);
    std::uint8_t readAsHexU8(const boost::string_view& value);

    std::int8_t readAsI8(const boost::string_view& value);
    std::int8_t readAsHexI8(const boost::string_view& value);
}}
