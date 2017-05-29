#pragma once 
#include <cstddef>

namespace swizzle { namespace parser {

    struct ParserStateContext
    {
        std::size_t CurrentEnumValue = 0;
    };
}}
