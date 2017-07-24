#include <swizzle/types/SafeStringStream.hpp>
#include <swizzle/Exceptions.hpp>

#include <boost/numeric/conversion/cast.hpp>
#include <limits>

namespace swizzle { namespace types {

    namespace {

        template<typename T>
        T ReadUnsigned(char const * const input, const std::size_t length)
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
                    throw InvalidStreamInput(std::string(input, length));
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
                    throw StreamInputCausesOverflow(std::string(input, length));
                }

                previous = t;
            }

            return t;
        }

        // convert std::uint16_t -> std::uint8_t, catch boost::bad_numeric_cast and raise our exception
        // instead.
        std::uint8_t convert_to_u8(const std::uint16_t v, char const * const input, const std::size_t length)
        {
            try
            {
                return boost::numeric_cast<std::uint8_t>(v);
            }
            catch(const boost::bad_numeric_cast&)
            {
                throw StreamInputCausesOverflow(std::string(input, length));
            }
        }

        // specialization for u8 which can rollover badly enough we can't detect it.
        template<>
        std::uint8_t ReadUnsigned(char const * const input, const std::size_t length)
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
                    throw InvalidStreamInput(std::string(input, length));
                }
            }

            std::uint16_t t = 0;
            std::uint16_t previous = 0;

            char const* spot = input;
            char const* end = input + length;

            for(; spot != end; ++spot)
            {
                t *= 10;
                t += *spot - '0';

                if((t < previous) && (previous != 0))
                {
                    throw StreamInputCausesOverflow(std::string(input, length));
                }

                previous = t;
            }

            return convert_to_u8(t, input, length);
        }


        template<typename T>
        T ReadSigned(char const * const input, const std::size_t length)
        {
            using Unsigned = typename std::make_unsigned<T>::type;
            const bool isNegative = input[0] == '-';

            const auto positiveResult = isNegative
                ? ReadUnsigned<Unsigned>(input + 1, length - 1)
                : ReadUnsigned<Unsigned>(input, length);

            // abs(min) == max + 1
            constexpr const auto min = static_cast<std::size_t>(std::numeric_limits<T>::max()) + 1;
            if(isNegative && (static_cast<std::size_t>(positiveResult) > min))
            {
                throw StreamInputCausesUnderflow(std::string(input, length));
            }

            if(!isNegative && (positiveResult > static_cast<Unsigned>(std::numeric_limits<T>::max())))
            {
                throw StreamInputCausesOverflow(std::string(input, length));
            }

            return isNegative
                ? static_cast<T>(positiveResult) * -1
                : static_cast<T>(positiveResult);
        }

        template<typename T>
        T Read(char const * const input, const std::size_t length)
        {
            static_assert(std::is_integral<T>::value, "T must be integral type.");

            return std::is_signed<T>::value
                ? ReadSigned<T>(input, length)
                : ReadUnsigned<T>(input, length);
        }

        template<typename T>
        T ReadHex(char const * const input, const std::size_t length)
        {
            static_assert(std::is_integral<T>::value, "T must be integral type.");

            if((input[0] != '0') || (input[1] != 'x'))
            {
                throw InvalidStreamInput(std::string(input, length));
            }

            // ensure input isn't too large for type
            if((length - 2) > (sizeof(T) * 2))
            {
                throw StreamInputCausesOverflow(std::string(input, length));
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
                    throw InvalidStreamInput(std::string(input, length));
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

    safe_istringstream::safe_istringstream(const boost::string_view& input)
        : input_(input)
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
            i = ReadHex<std::uint8_t>(input_.data(), input_.length());
        }
        else
        {
            i = Read<std::uint8_t>(input_.data(), input_.length());
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
            i = ReadHex<std::int8_t>(input_.data(), input_.length());
        }
        else
        {
            i = Read<std::int8_t>(input_.data(), input_.length());
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
            i = ReadHex<std::uint16_t>(input_.data(), input_.length());
        }
        else
        {
            i = Read<std::uint16_t>(input_.data(), input_.length());
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
            i = ReadHex<std::int16_t>(input_.data(), input_.length());
        }
        else
        {
            i = Read<std::int16_t>(input_.data(), input_.length());
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
            i = ReadHex<std::uint32_t>(input_.data(), input_.length());
        }
        else
        {
            i = Read<std::uint32_t>(input_.data(), input_.length());
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
            i = ReadHex<std::int32_t>(input_.data(), input_.length());
        }
        else
        {
            i = Read<std::int32_t>(input_.data(), input_.length());
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
            i = ReadHex<std::uint64_t>(input_.data(), input_.length());
        }
        else
        {
            i = Read<std::uint64_t>(input_.data(), input_.length());
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
            i = ReadHex<std::int64_t>(input_.data(), input_.length());
        }
        else
        {
            i = Read<std::int64_t>(input_.data(), input_.length());
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
