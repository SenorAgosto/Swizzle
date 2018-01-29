#include <swizzle/types/SetValueFromChar.hpp>
#include <swizzle/Exceptions.hpp>

namespace swizzle { namespace types {

    std::uint64_t setValueFromChar(const boost::string_view& underlying, const boost::string_view& value)
    {
        if((underlying == "u8") || (underlying == "i8")   ||
           (underlying == "u16") || (underlying == "i16") ||
           (underlying == "u32") || (underlying == "i32") ||
           (underlying == "u64") || (underlying == "i64"))
        {
            return value[0];
        }

        throw ParserError("Encountered unknown enum type:" + underlying.to_string());
    }
}}
