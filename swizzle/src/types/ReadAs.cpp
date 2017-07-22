#include <swizzle/types/ReadAs.hpp>

namespace swizzle { namespace types {

    std::uint8_t readAsU8(const boost::string_view& value)
    {
        std::istringstream is(std::string(value.data(), value.length()));

        std::uint16_t t = 0;
        is >> t;

        if(!is.eof())
        {
            throw StreamNotFullyConsumed(value);
        }

        return boost::numeric_cast<std::uint8_t>(t);
    }

    std::uint8_t readAsHexU8(const boost::string_view& value)
    {
        if(value.length() != 4)
        {
            throw std::out_of_range("'value.length() != 4' in readAsHexU8()");
        }

        std::istringstream is(std::string(value.data() + 2, value.length() - 2));

        std::uint16_t t = 0;
        is >> std::hex >> t;

        if(!is.eof())
        {
            throw StreamNotFullyConsumed(value);
        }

        return boost::numeric_cast<std::uint8_t>(t);
    }

    std::int8_t readAsI8(const boost::string_view& value)
    {
        std::istringstream is(std::string(value.data(), value.length()));

        std::int16_t t = 0;
        is >> t;

        if(!is.eof())
        {
            throw StreamNotFullyConsumed(value);
        }

        return boost::numeric_cast<std::int8_t>(t);
    }

    std::int8_t readAsHexI8(const boost::string_view& value)
    {
        if(value.length() != 4)
        {
            throw std::out_of_range("'value.length() != 4' in readAsU8<isHex=true>()");
        }

        // trim leading 0x
        std::istringstream is(std::string(value.data() + 2, value.length() - 2));

        std::int16_t t = 0;
        is >> std::hex >> t;

        if(!is.eof())
        {
            throw StreamNotFullyConsumed(value);
        }

        return boost::numeric_cast<std::int8_t>(t);
    }
}}
