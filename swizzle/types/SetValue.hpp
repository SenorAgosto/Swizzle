#pragma once 
#include <swizzle/Exceptions.hpp>
#include <swizzle/types/ReadAs.hpp>

namespace swizzle { namespace types {

    template<bool isHex = false>
    EnumValueType setValue(const boost::string_view& underlying, const boost::string_view& value)
    {
        if(underlying == "u8")
        {
            return readAsU8<isHex>(value);
        }

        if(underlying == "i8")
        {
            return readAsI8<isHex>(value);
        }

        if(underlying == "u16")
        {
            return readAs<std::uint16_t, isHex>(value);
        }

        if(underlying == "i16")
        {
            return readAs<std::int16_t, isHex>(value);
        }

        if(underlying == "u32")
        {
            return readAs<std::uint32_t, isHex>(value);
        }

        if(underlying == "i32")
        {
            return readAs<std::int32_t, isHex>(value);
        }

        if(underlying == "u64")
        {
            return readAs<std::uint64_t, isHex>(value);
        }

        if(underlying == "i64")
        {
            return readAs<std::int64_t, isHex>(value);
        }

        throw ParserError("Encountered unknown enum type:" + underlying.to_string());
    }
}}
