#pragma once
#include <swizzle/types/EnumValue.hpp>
#include <swizzle/types/EnumValueType.hpp>

#include <cstddef>
#include <string>

namespace swizzle { namespace parser {

    struct ParserStateContext
    {
        std::string CurrentNamespace;
        std::size_t CurrentBitfieldBit = 0;
        types::EnumValue CurrentEnumValue = types::EnumValue(types::EnumValueType(std::uint64_t(0)));
    };
}}
