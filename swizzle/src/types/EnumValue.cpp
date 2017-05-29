#include <swizzle/types/EnumValue.hpp>

#include <boost/numeric/conversion/cast.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/static_visitor.hpp>
#include <cstddef>
#include <stdexcept>
#include <sstream>

namespace swizzle { namespace types {

    EnumValue::EnumValue()
        : value_(std::uint64_t(0))
    {
    }

    EnumValue::EnumValue(const EnumValueType& value)
        : value_(value)
    {
    }

    namespace {
        template<class T>
        T readAs(const boost::string_view& value)
        {
            std::istringstream is(value.data(), value.length());

            T t;
            is >> t;

            return t;
        }
    }

    EnumValue::EnumValue(const lexer::TokenInfo& underlying, const lexer::TokenInfo& value)
        : value_(std::uint64_t(0))
    {
        // [ARG]: TODO: once adapted by boost, use safe_numerics
        // the code below likely allows values to roll over when
        // they're larger than the input type, safe_numerics
        // will detect the error & throw

        // based on the underlying type, change the variant
        const auto& underlyingType = underlying.token().value();
        if(underlyingType == "u8")
        {
            value_ = readAs<std::uint8_t>(value.token().value());
            return;
        }

        if(underlyingType == "i8")
        {
            value_ = readAs<std::int8_t>(value.token().value());
            return;
        }

        if(underlyingType == "u16")
        {
            value_ = readAs<std::uint16_t>(value.token().value());
            return;
        }

        if(underlyingType == "i16")
        {
            value_ = readAs<std::int16_t>(value.token().value());
            return;
        }

        if(underlyingType == "u32")
        {
            value_ = readAs<std::uint32_t>(value.token().value());
            return;
        }

        if(underlyingType == "i32")
        {
            value_ = readAs<std::int32_t>(value.token().value());
            return;
        }

        if(underlyingType == "u64")
        {
            value_ = readAs<std::uint64_t>(value.token().value());
            return;
        }

        if(underlyingType == "i64")
        {
            value_ = readAs<std::int64_t>(value.token().value());
            return;
        }
    }

    const EnumValueType& EnumValue::value() const
    {
        return value_;
    }

    namespace {
        struct incrementor : public boost::static_visitor<>
        {
            // [ARG]: TODO: adapt safe_numerics once it is accepted to boost,
            // the implementation becomes just '++i;'
            template<class T>
            void operator()(T& i) const
            {
                if(i == std::numeric_limits<std::uint8_t>::max())
                {
                    throw std::out_of_range("incrementing value would overflow value type: uint8_t");
                }

                ++i;
            }
        };
    }

    void EnumValue::increment()
    {
        boost::apply_visitor(incrementor(), value_);
    }
}}
