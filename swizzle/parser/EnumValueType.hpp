#pragma once
#include <boost/variant.hpp>
#include <swizzle/platform/safe_integer.hpp>

namespace swizzle { namespace parser {

    using swizzle::platform::safe;

    using EnumValueType = boost::variant<
          safe<std::uint8_t>
        , safe<std::int8_t>
        , safe<std::uint16_t>
        , safe<std::int16_t>
        , safe<std::uint32_t>
        , safe<std::int32_t>
        , safe<std::uint64_t>
        , safe<std::int64_t>
    >;
}}
