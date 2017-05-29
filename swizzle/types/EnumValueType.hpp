#pragma once
#include <boost/variant.hpp>
#include <cstdint>

namespace swizzle { namespace types {

    using EnumValueType = boost::variant<
          std::uint8_t
        , std::int8_t
        , std::uint16_t
        , std::int16_t
        , std::uint32_t
        , std::int32_t
        , std::uint64_t
        , std::int64_t
    >;
}}
