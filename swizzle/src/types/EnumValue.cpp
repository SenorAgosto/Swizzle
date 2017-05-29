#include <swizzle/types/EnumValue.hpp>

#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/static_visitor.hpp>
#include <limits>
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
