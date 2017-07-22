#include <swizzle/types/SetValue.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/types/ReadAs.hpp>

namespace swizzle { namespace types {

    EnumValueType setValue(const boost::string_view& underlying, const boost::string_view& value)
    {
        if(underlying == "u8")
        {
            return readAsU8(value);
        }

        if(underlying == "i8")
        {
            return readAsI8(value);
        }

        if(underlying == "u16")
        {
            return readAs<std::uint16_t>(value);
        }

        if(underlying == "i16")
        {
            return readAs<std::int16_t>(value);
        }

        if(underlying == "u32")
        {
            return readAs<std::uint32_t>(value);
        }

        if(underlying == "i32")
        {
            return readAs<std::int32_t>(value);
        }

        if(underlying == "u64")
        {
            return readAs<std::uint64_t>(value);
        }

        if(underlying == "i64")
        {
            return readAs<std::int64_t>(value);
        }

        throw ParserError("Encountered unknown enum type:" + underlying.to_string());
    }

    EnumValueType setValue(const boost::string_view& underlying, const boost::string_view& value, const isHexTag&)
    {
        if(underlying == "u8")
        {
            return readAsHexU8(value);
        }

        if(underlying == "i8")
        {
            return readAsHexI8(value);
        }

        if(underlying == "u16")
        {
            return readAsHex<std::uint16_t>(value);
        }

        if(underlying == "i16")
        {
            return readAsHex<std::int16_t>(value);
        }

        if(underlying == "u32")
        {
            return readAsHex<std::uint32_t>(value);
        }

        if(underlying == "i32")
        {
            return readAsHex<std::int32_t>(value);
        }

        if(underlying == "u64")
        {
            return readAsHex<std::uint64_t>(value);
        }

        if(underlying == "i64")
        {
            return readAsHex<std::int64_t>(value);
        }

        throw ParserError("Encountered unknown enum type:" + underlying.to_string());
    }
}}
