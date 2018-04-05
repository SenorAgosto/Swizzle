#pragma once
#include <string>

namespace swizzle { namespace types {

    enum class SymbolType
    {
        Bitfield,
        BitfieldField,
        Enum,
        EnumField,
        Extern,
        Import,
        Struct,
        StructField,
        Namespace,
        TypeAlias,
        VariableBlock,
    };

    std::string to_string(const SymbolType type);
}}
