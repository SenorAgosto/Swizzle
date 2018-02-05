#pragma once

namespace swizzle { namespace parser {

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
