#pragma once

#include <boost/utility/string_view.hpp>
#include <cstdint>
#include <stdexcept>
#include <sstream>
#include <type_traits>

namespace swizzle { namespace types {

    // @isHex should be true if the stream has a hex value in the form 0x01
    template<class T, bool isHex>
    T readAs(const boost::string_view& value)
    {
        static_assert(! std::is_same<T, std::uint8_t>::value, "Cannot instantiate this function with std::uint8_t, use the overloads readAsU8() and readAsI8() instead.");
        
        if(isHex)
        {
            // trim leading 0x
            std::istringstream is(value.data() + 2, value.length() - 2);

            T t = 0;
            is >> std::hex >> t;

            return t;
        }

        std::istringstream is(value.data(), value.length());

        T t = 0;
        is >> t;

        return t;
    }

    template<bool isHex>
    std::uint8_t readAsU8(const boost::string_view& value)
    {
        if(isHex)
        {
            if(value.length() != 4)
            {
                throw std::out_of_range("'value.length() != 4' in readAsU8<isHex=true>()");
            }

            std::istringstream is(value.data() + 2, value.length() - 2);

            std::uint16_t t = 0;
            is >> std::hex >> t;

            return static_cast<std::uint8_t>(t);
        }

        std::istringstream is(value.data(), value.length());

        std::uint16_t t = 0;
        is >> t;

        return static_cast<std::uint8_t>(t);
    }

    template<bool isHex>
    std::int8_t readAsI8(const boost::string_view& value)
    {
        if(isHex)
        {
            if(value.length() != 4)
            {
                throw std::out_of_range("'value.length() != 4' in readAsU8<isHex=true>()");
            }

            std::istringstream is(value.data() + 2, value.length() - 2);

            std::int16_t t = 0;
            is >> std::hex >> t;

            return static_cast<std::int8_t>(t);
        }

        std::istringstream is(value.data(), value.length());

        std::int16_t t = 0;
        is >> t;

        return static_cast<std::int8_t>(t);
    }
}}
