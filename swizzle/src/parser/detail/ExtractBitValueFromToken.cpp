#include <swizzle/parser/detail/ExtractBitValueFromToken.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/types/ReadAs.hpp>

#include <boost/numeric/conversion/cast.hpp>
#include <cstddef>
#include <cstdint>

namespace swizzle { namespace parser { namespace detail {

    std::intmax_t extractBitValueFromToken(const boost::string_view& underlying, const lexer::TokenInfo& info)
    {
        try
        {
            if(underlying == "u8")
            {
                const auto value = boost::numeric_cast<std::intmax_t>(types::readAs<std::uint8_t>(info));
                if(value > 7)
                {
                    throw SyntaxError("Bitfield bit value exceeds range supported by underlying type (" + underlying.to_string() + ")", info);
                }

                return value;
            }

            if(underlying == "u16")
            {
                const auto value = boost::numeric_cast<std::intmax_t>(types::readAs<std::uint16_t>(info));
                if(value > 15)
                {
                    throw SyntaxError("Bitfield bit value exceeds range supported by underlying type (" + underlying.to_string() + ")", info);
                }

                return value;
            }

            if(underlying == "u32")
            {
                const auto value = boost::numeric_cast<std::intmax_t>(types::readAs<std::uint32_t>(info));
                if(value > 31)
                {
                    throw SyntaxError("Bitfield bit value exceeds range supported by underlying type (" + underlying.to_string() + ")", info);
                }

                return value;
            }

            if(underlying == "u64")
            {
                const auto value = boost::numeric_cast<std::intmax_t>(types::readAs<std::uint64_t>(info));
                if(value > 63)
                {
                    throw SyntaxError("Bitfield bit value exceeds range supported by underlying type (" + underlying.to_string() + ")", info);
                }

                return value;
            }
        }
        catch(const boost::bad_numeric_cast&)
        {
            throw SyntaxError("Internal parser error, unexpected value overflow.", info);
        }
        catch(const StreamInputCausesOverflow&)
        {
            throw SyntaxError("Bitfield bit value exceeds range supported by underlying type (" + underlying.to_string() + ")", info);
        }
        catch(const StreamInputCausesUnderflow&)
        {
            throw ParserError("Internal parser error, unexpected value underflow.");
        }
        catch(const InvalidStreamInput&)
        {
            throw SyntaxError("Bitfield bit value contains invalid character.", info);
        }

        throw ParserError("Internal parser error, unexpected underlying type found parsing bitfield");
    }

}}}
