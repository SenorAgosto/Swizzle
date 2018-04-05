#include <swizzle/types/SymbolType.hpp>
#include <stdexcept>

namespace swizzle { namespace types {

    std::string to_string(const SymbolType type)
    {
        switch(type)
        {
        case SymbolType::Bitfield: return "SymbolType::Bitfield";
        case SymbolType::BitfieldField: return "SymbolType::BitfieldField";
        case SymbolType::Enum: return "SymbolType::Enum";
        case SymbolType::EnumField: return "SymbolType::EnumField";
        case SymbolType::Extern: return "SymbolType::Extern";
        case SymbolType::Import: return "SymbolType::Import";
        case SymbolType::Struct: return "SymbolType::Struct";
        case SymbolType::StructField: return "SymbolType::StructField";
        case SymbolType::Namespace: return "SymbolType::Namespace";
        case SymbolType::TypeAlias: return "SymbolType::TypeAlias";
        case SymbolType::VariableBlock: return "SymbolType::VariableBlock"; 
        }

        throw std::runtime_error("Unknown SymbolType");
    }
}}

