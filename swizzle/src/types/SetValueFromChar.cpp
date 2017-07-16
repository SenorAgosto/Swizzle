#include <swizzle/types/SetValueFromChar.hpp>
#include <swizzle/Exceptions.hpp>

namespace swizzle { namespace types {

    EnumValueType setValueFromChar(const boost::string_view& underlying, const boost::string_view& value)
    {
        if(underlying == "u8")
        {
            return static_cast<std::uint8_t>(value[0]);
        }

        if(underlying == "i8")
        {
            return static_cast<std::int8_t>(value[0]);
        }

        if(underlying == "u16")
        {
            return static_cast<std::uint16_t>(value[0]);
        }

        if(underlying == "i16")
        {
            return static_cast<std::int16_t>(value[0]);
        }

        if(underlying == "u32")
        {
            return static_cast<std::uint32_t>(value[0]);
        }

        if(underlying == "i32")
        {
            return static_cast<std::int32_t>(value[0]);
        }

        if(underlying == "u64")
        {
            return static_cast<std::uint64_t>(value[0]);
        }

        if(underlying == "i64")
        {
            return static_cast<std::int64_t>(value[0]);
        }

        throw ParserError("Encountered unknown enum type:" + underlying.to_string());
    }
}}
