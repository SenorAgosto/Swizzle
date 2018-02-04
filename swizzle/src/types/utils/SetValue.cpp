#include <swizzle/types/utils/SetValue.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/types/ReadAs.hpp>

namespace swizzle { namespace types { namespace utils {

    std::uint64_t setValue(const boost::string_view& underlying, const lexer::TokenInfo& token, const std::string& errorMessage)
    {
        
        if(underlying == "u8")
        {
            return readAs<std::uint8_t>(token);
        }

        if(underlying == "i8")
        {
            return readAs<std::int8_t>(token);
        }

        if(underlying == "u16")
        {
            return readAs<std::uint16_t>(token);
        }

        if(underlying == "i16")
        {
            return readAs<std::int16_t>(token);
        }

        if(underlying == "u32")
        {
            return readAs<std::uint32_t>(token);
        }

        if(underlying == "i32")
        {
            return readAs<std::int32_t>(token);
        }

        if(underlying == "u64")
        {
            return readAs<std::uint64_t>(token);
        }

        if(underlying == "i64")
        {
            readAs<std::int64_t>(token);
        }

        throw SyntaxError(errorMessage + ": " + underlying.to_string(), token);
    }

    std::uint64_t setValue(const boost::string_view& underlying, const lexer::TokenInfo& token, const isHexTag&, const std::string& errorMessage)
    {
        if(underlying == "u8")
        {
            return readAsHex<std::uint8_t>(token);
        }

        if(underlying == "i8")
        {
            return readAsHex<std::int8_t>(token);
        }

        if(underlying == "u16")
        {
            return readAsHex<std::uint16_t>(token);
        }

        if(underlying == "i16")
        {
            return readAsHex<std::int16_t>(token);
        }

        if(underlying == "u32")
        {
            return readAsHex<std::uint32_t>(token);
        }

        if(underlying == "i32")
        {
            return readAsHex<std::int32_t>(token);
        }

        if(underlying == "u64")
        {
            return readAsHex<std::uint64_t>(token);
        }

        if(underlying == "i64")
        {
            return readAsHex<std::int64_t>(token);
        }

        throw SyntaxError(errorMessage + ": " + underlying.to_string(), token);
    }
}}}
