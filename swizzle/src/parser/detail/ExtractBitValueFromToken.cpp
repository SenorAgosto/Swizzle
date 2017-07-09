#include <swizzle/parser/detail/ExtractBitValueFromToken.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

#include <boost/lexical_cast.hpp>
#include <cstdint>

namespace swizzle { namespace parser { namespace detail {

    std::size_t extractBitValueFromToken(const boost::string_view& underlying, const lexer::TokenInfo& info)
    {
        try
        {
            if(underlying == "u8")
            {
                return boost::lexical_cast<std::uint8_t>(info.token().value());
            }

            if(underlying == "u16")
            {
                return boost::lexical_cast<std::uint16_t>(info.token().value());
            }

            if(underlying == "u32")
            {
                return boost::lexical_cast<std::uint32_t>(info.token().value());
            }

            if(underlying == "u64")
            {
                return boost::lexical_cast<std::uint64_t>(info.token().value());
            }
        }
        catch(const boost::bad_lexical_cast&)
        {
            throw SyntaxError("Bitfield bit value exceeds range supported by underlying type (" + underlying.to_string() + ")", info);
        }

        throw ParserError("Internal parser error, unexpected underlying type found parsing bitfield");
    }


}}}
