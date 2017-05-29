#pragma once
#include <swizzle/types/EnumValue.hpp>

#include <cstddef>
#include <string>

namespace swizzle { namespace parser {

    struct ParserStateContext
    {
        std::string CurrentNamespace;
        types::EnumValue CurrentEnumValue;
    };
}}
