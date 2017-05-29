#pragma once 
#include <cstddef>
#include <string>

namespace swizzle { namespace parser {

    struct ParserStateContext
    {
        std::string CurrentNamespace;
        std::size_t CurrentEnumValue = 0;
    };
}}
