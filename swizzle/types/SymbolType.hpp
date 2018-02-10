#pragma once

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
}}
