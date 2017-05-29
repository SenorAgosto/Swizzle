#include <swizzle/parser/EnumValue.hpp>
#include <swizzle/platform/safe_integer.hpp>

#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/static_visitor.hpp>
#include <cstddef>
#include <sstream>

namespace swizzle { namespace parser {

    using platform::safe;

    EnumValue::EnumValue()
        : value_(safe<std::uint64_t>(0))
    {
    }

    namespace {
        template<class T>
        inline
        T readAs(const boost::string_view& value)
        {
            std::istringstream is(value.data(), value.length());

            T t = 0;
            is >> t;

            return t;
        }
    }

    EnumValue::EnumValue(const lexer::TokenInfo& info, const lexer::TokenInfo& underlying, const lexer::TokenInfo& value)
        : info_(info)
        , underlying_(underlying)
        , value_(safe<std::uint64_t>(0))
    {
        // based on the underlying type, change the variant
        const auto& underlyingType = underlying_.token().value();
        if(underlyingType == "u8")
        {
            value_ = readAs<safe<std::uint8_t>>(value.token().value());
            return;
        }

        if(underlyingType == "i8")
        {
            value_ = readAs<safe<std::int8_t>>(value.token().value());
            return;
        }

        if(underlyingType == "u16")
        {
            value_ = readAs<safe<std::uint16_t>>(value.token().value());
            return;
        }

        if(underlyingType == "i16")
        {
            value_ = readAs<safe<std::int16_t>>(value.token().value());
            return;
        }

        if(underlyingType == "u32")
        {
            value_ = readAs<safe<std::uint32_t>>(value.token().value());
            return;
        }

        if(underlyingType == "i32")
        {
            value_ = readAs<safe<std::int32_t>>(value.token().value());
            return;
        }

        if(underlyingType == "u64")
        {
            value_ = readAs<safe<std::uint64_t>>(value.token().value());
            return;
        }

        if(underlyingType == "i64")
        {
            value_ = readAs<safe<std::int64_t>>(value.token().value());
            return;
        }
    }

    EnumValue::EnumValue(const lexer::TokenInfo& info, const lexer::TokenInfo& underlying, const EnumValueType& value)
        : info_(info)
        , underlying_(underlying)
        , value_(value)
    {
    }

    const EnumValueType& EnumValue::value() const
    {
        return value_;
    }

    namespace {
        struct incrementor : public boost::static_visitor<>
        {
            void operator()(safe<std::uint8_t>& i) const { ++i; }
            void operator()(safe<std::int8_t>& i) const { ++i; }
            void operator()(safe<std::uint16_t>& i) const { ++i; }
            void operator()(safe<std::int16_t>& i) const { ++i; }
            void operator()(safe<std::uint32_t>& i) const { ++i; }
            void operator()(safe<std::int32_t>& i) const { ++i; }
            void operator()(safe<std::uint64_t>& i) const { ++i; }
            void operator()(safe<std::int64_t>& i) const { ++i; }
        };
    }

    void EnumValue::increment()
    {
        boost::apply_visitor(incrementor(), value_);
    }
}}
