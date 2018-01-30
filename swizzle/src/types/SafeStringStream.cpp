#include <swizzle/types/SafeStringStream.hpp>
#include <swizzle/Exceptions.hpp>

#include <limits>

namespace swizzle { namespace types {

    namespace {

        template<typename T>
        T ReadUnsigned(const lexer::TokenInfo& token, char const * const input, const std::size_t length)
        {
            // validate input.
            for(std::size_t i = 0; i < length; ++i)
            {
                if(std::isdigit(input[i]))
                {
                    continue;
                }
                else
                {
                    throw InvalidStreamInput(token, std::string(input, length));
                }
            }

            T t = 0;
            T previous = 0;

            char const* spot = input;
            char const* end = input + length;

            for(; spot != end; ++spot)
            {
                t *= 10;
                t += *spot - '0';

                if((t < previous) && (previous != 0))
                {
                    throw StreamInputCausesOverflow(token, std::string(input, length));
                }

                previous = t;
            }

            return t;
        }

        // specialization for u8 which can rollover badly enough we can't detect it.
        template<>
        std::uint8_t ReadUnsigned(const lexer::TokenInfo& token, char const * const input, const std::size_t length)
        {
            // validate input
            for(std::size_t i = 0; i < length; ++i)
            {
                if(std::isdigit(input[i]))
                {
                    continue;
                }
                else
                {
                    throw InvalidStreamInput(token, std::string(input, length));
                }
            }

            std::uint16_t t = 0;

            char const* spot = input;
            char const* end = input + length;

            for(; spot != end; ++spot)
            {
                t *= 10;
                t += *spot - '0';

                if(t > 255)
                {
                    throw StreamInputCausesOverflow(token, std::string(input, length));
                }
            }

            return static_cast<std::uint8_t>(t);
        }


        template<typename T>
        T ReadSigned(const lexer::TokenInfo& token, char const * const input, const std::size_t length)
        {
            using Unsigned = typename std::make_unsigned<T>::type;
            const bool isNegative = input[0] == '-';

            const auto positiveResult = isNegative
                ? ReadUnsigned<Unsigned>(token, input + 1, length - 1)
                : ReadUnsigned<Unsigned>(token, input, length);

            // abs(min) == max + 1
            constexpr const auto min = static_cast<std::size_t>(std::numeric_limits<T>::max()) + 1;
            if(isNegative && (static_cast<std::size_t>(positiveResult) > min))
            {
                throw StreamInputCausesUnderflow(token, std::string(input, length));
            }

            if(!isNegative && (positiveResult > static_cast<Unsigned>(std::numeric_limits<T>::max())))
            {
                throw StreamInputCausesOverflow(token, std::string(input, length));
            }

            return isNegative
                ? static_cast<T>(positiveResult) * -1
                : static_cast<T>(positiveResult);
        }

        template<typename T>
        T Read(const lexer::TokenInfo& token, char const * const input, const std::size_t length)
        {
            static_assert(std::is_integral<T>::value, "T must be integral type.");

            return std::is_signed<T>::value
                ? ReadSigned<T>(token, input, length)
                : ReadUnsigned<T>(token, input, length);
        }

        template<typename T>
        T ReadHex(const lexer::TokenInfo& token, char const * const input, const std::size_t length)
        {
            static_assert(std::is_integral<T>::value, "T must be integral type.");

            if((input[0] != '0') || (input[1] != 'x'))
            {
                throw InvalidStreamInput(token, std::string(input, length));
            }

            // ensure input isn't too large for type
            if((length - 2) > (sizeof(T) * 2))
            {
                throw StreamInputCausesOverflow(token, std::string(input, length));
            }

            // validate input
            for(std::size_t i = 2; i < length; ++i)
            {
                if(std::isxdigit(input[i]))
                {
                    continue;
                }
                else
                {
                    throw InvalidStreamInput(token, std::string(input, length));
                }
            }

            T t = 0;
            T temp = 0;
            std::size_t shift = 0;

            char const * const end = input + 1;
            char const * spot = input + length - 1;

            for(; spot != end; --spot)
            {
                const char c = *spot;

                if(c >= 'a')
                {
                    temp |= c - 'a' + 10;
                }
                else
                if(c >= 'A')
                {
                    temp |= c - 'A' + 10;
                }
                else
                {
                    temp |= c - '0';
                }

                temp = temp << shift;
                shift += 4;

                t |= temp;
                temp = 0;
            }

            return t;
        }

    }

    safe_istringstream::safe_istringstream(const lexer::TokenInfo& token)
        : token_(token)
        , input_(token.token().value())
        , hex_(false)
    {
    }

    safe_istringstream& safe_istringstream::operator>>(std::uint8_t& i)
    {
        if(input_.empty())
        {
            throw StreamEmpty();
        }

        if(hex_)
        {
            i = ReadHex<std::uint8_t>(token_, input_.data(), input_.length());
        }
        else
        {
            i = Read<std::uint8_t>(token_, input_.data(), input_.length());
        }

        return *this;
    }

    safe_istringstream& safe_istringstream::operator>>(std::int8_t& i)
    {
        if(input_.empty())
        {
            throw StreamEmpty();
        }

        if(hex_)
        {
            i = ReadHex<std::int8_t>(token_, input_.data(), input_.length());
        }
        else
        {
            i = Read<std::int8_t>(token_, input_.data(), input_.length());
        }

        return *this;
    }

    safe_istringstream& safe_istringstream::operator>>(std::uint16_t& i)
    {
        if(input_.empty())
        {
            throw StreamEmpty();
        }

        if(hex_)
        {
            i = ReadHex<std::uint16_t>(token_, input_.data(), input_.length());
        }
        else
        {
            i = Read<std::uint16_t>(token_, input_.data(), input_.length());
        }

        return *this;
    }

    safe_istringstream& safe_istringstream::operator>>(std::int16_t& i)
    {
        if(input_.empty())
        {
            throw StreamEmpty();
        }

        if(hex_)
        {
            i = ReadHex<std::int16_t>(token_, input_.data(), input_.length());
        }
        else
        {
            i = Read<std::int16_t>(token_, input_.data(), input_.length());
        }

        return *this;
    }

    safe_istringstream& safe_istringstream::operator>>(std::uint32_t& i)
    {
        if(input_.empty())
        {
            throw StreamEmpty();
        }

        if(hex_)
        {
            i = ReadHex<std::uint32_t>(token_, input_.data(), input_.length());
        }
        else
        {
            i = Read<std::uint32_t>(token_, input_.data(), input_.length());
        }

        return *this;
    }

    safe_istringstream& safe_istringstream::operator>>(std::int32_t& i)
    {
        if(input_.empty())
        {
            throw StreamEmpty();
        }

        if(hex_)
        {
            i = ReadHex<std::int32_t>(token_, input_.data(), input_.length());
        }
        else
        {
            i = Read<std::int32_t>(token_, input_.data(), input_.length());
        }

        return *this;
    }

    safe_istringstream& safe_istringstream::operator>>(std::uint64_t& i)
    {
        if(input_.empty())
        {
            throw StreamEmpty();
        }

        if(hex_)
        {
            i = ReadHex<std::uint64_t>(token_, input_.data(), input_.length());
        }
        else
        {
            i = Read<std::uint64_t>(token_, input_.data(), input_.length());
        }

        return *this;
    }

    safe_istringstream& safe_istringstream::operator>>(std::int64_t& i)
    {
        if(input_.empty())
        {
            throw StreamEmpty();
        }

        if(hex_)
        {
            i = ReadHex<std::int64_t>(token_, input_.data(), input_.length());
        }
        else
        {
            i = Read<std::int64_t>(token_, input_.data(), input_.length());
        }

        return *this;
    }

    safe_istringstream& operator>>(safe_istringstream& is, const HexStreamTag&)
    {
        static const bool isHex = true;
        is.hex_stream(isHex);

        return is;
    }
}}
