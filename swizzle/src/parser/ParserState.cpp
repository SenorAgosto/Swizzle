#include <swizzle/parser/ParserState.hpp>

namespace swizzle { namespace parser {

    std::ostream& operator<<(std::ostream& os, const ParserState state)
    {
        switch(state)
        {
        case ParserState::Init: return os << "ParserState::Init";
        case ParserState::StartNamespace: return os << "ParserState::StartNamespace";
        case ParserState::NamespaceValue: return os << "ParserState::NamespaceValue";
        case ParserState::NamespaceFirstColon: return os << "ParserState::NamespaceFirstColon";
        case ParserState::TranslationUnitMain: return os << "ParserState::TranslationUnitMain";
        case ParserState::StartImport: return os << "ParserState::StartImport";
        case ParserState::ImportValue: return os << "ParserState::ImportValue";
        case ParserState::ImportFirstColon: return os << "ParserState::ImportFirstColon";
        case ParserState::StartExtern: return os << "ParserState::StartExtern";
        case ParserState::ExternValue: return os << "ParserState::ExternValue";
        case ParserState::ExternFirstColon: return os << "ParserState::ExternFirstColon";
        case ParserState::StartUsing: return os << "ParserState::StartUsing";
        case ParserState::UsingName: return os << "ParserState::UsingName";
        case ParserState::UsingReadEqual: return os << "ParserState::UsingReadEqual";
        case ParserState::UsingTypeRead: return os << "ParserState::UsingTypeRead";
        case ParserState::StartEnum: return os << "ParserState::StartEnum";
        case ParserState::EnumName: return os << "ParserState::EnumName";
        case ParserState::EnumColonRead: return os << "ParserState::EnumColonRead";
        case ParserState::EnumUnderlyingType: return os << "ParserState::EnumUnderlyingType";
        case ParserState::EnumStartScope: return os << "ParserState::EnumStartScope";
        case ParserState::EnumField: return os << "ParserState::EnumField";
        case ParserState::EnumFieldEqualRead: return os << "ParserState::EnumFieldEqualRead";
        case ParserState::EnumFieldValueRead: return os << "ParserState::EnumFieldValueRead";
        case ParserState::StartBitfield: return os << "ParserState::StartBitfield";
        case ParserState::BitfieldName: return os << "ParserState::BitfieldName";
        case ParserState::BitfieldColonRead: return os << "ParserState::BitfieldColonRead";
        case ParserState::BitfieldUnderlyingType: return os << "ParserState::BitfieldUnderlyingType";
        case ParserState::BitfieldStartScope: return os << "ParserState::BitfieldStartScope";
        case ParserState::BitfieldField: return os << "ParserState::BitfieldField";
        case ParserState::BitfieldFieldColonRead: return os << "ParserState::BitfieldFieldColonRead";
        case ParserState::BitfieldStartPosition: return os << "ParserState::BitfieldStartPosition";
        case ParserState::BitfieldFirstDot: return os << "ParserState::BitfieldFirstDot";
        case ParserState::BitfieldSecondDot: return os << "ParserState::BitfieldSecondDot";
        case ParserState::BitfieldEndPosition: return os << "ParserState::BitfieldEndPosition";
        case ParserState::UsingFirstColon: return os << "ParserState::UsingFirstColon";
        case ParserState::UsingSecondColon: return os << "ParserState::UsingSecondColon";
        case ParserState::StartStruct: return os << "ParserState::StartStruct";
        case ParserState::StructBaseColonRead: return os << "ParserState::StructBaseColonRead";
        case ParserState::StructBaseNameOrNamespaceRead: return os << "ParserState::StructBaseNameOrNamespaceRead";
        case ParserState::StructBaseNameOrNamespaceFirstColonRead: return os << "ParserState::StructBaseNameOrNamespaceFirstColonRead";
        case ParserState::StructName: return os  << "ParserState::StructName";
        case ParserState::StructStartScope: return os << "ParserState::StructStartScope";
        case ParserState::StructFieldEnumOrNamespaceRead: return os << "ParserState::StructFieldEnumOrNamespaceRead";
        case ParserState::StructFieldEnumOrNamespaceFirstColonRead: return os << "ParserState::StructFieldEnumOrNamespaceFirstColonRead";
        case ParserState::StructFieldEnumValueRead: return os << "ParserState::StructFieldEnumOrNamespaceFirstColonRead";
        case ParserState::StructFieldLabel: return os << "ParserState::StructFieldLabel";
        case ParserState::StructFieldNamespaceOrType: return os << "ParserState::StructFieldNamespaceOrType";
        case ParserState::StructFieldNamespaceFirstColon: return os << "ParserState::StructFieldNamespaceFirstColon";
        case ParserState::StructFieldNamespaceSecondColon: return os << "ParserState::StructFieldNamespaceSecondColon";
        case ParserState::StructFieldName: return os << "ParserState::StructFieldName";
        case ParserState::StructFieldEqualRead: return os << "ParserState::StructFieldEqualRead";
        case ParserState::StructFieldValueRead: return os << "ParserState::StructFieldValueRead";
        case ParserState::StructStartArray: return os << "ParserState::StructStartArray";
        case ParserState::StructArray: return os << "ParserState::StructArray";
        case ParserState::StructVector: return os << "ParserState::StructVector";
        case ParserState::StructVectorNestedOnMember: return os << "ParserState::StructVectorNestedOnMember";
        case ParserState::StructEndArrayOrVector: return os << "ParserState::StructEndArrayOrVector";
        case ParserState::StructStartVariableBlock: return os << "ParserState::StructStartVariableBlock";
        case ParserState::StructVariableBlockColonRead: return os << "ParserState::StructVariableBlockColonRead";
        case ParserState::StructVariableBlockOnField: return os << "ParserState::StructVariableBlockOnField";
        case ParserState::StructVariableBlockOnNestedField: return os << "ParserState::StructVariableBlockOnNestedField";
        case ParserState::StructVariableBlockBeginCases: return os << "ParserState::StructVariableBlockBeginCases";
        case ParserState::StructVariableBlockCaseValue: return os << "ParserState::StructVariableBlockCaseValue";
        case ParserState::StructVariableBlockCaseValueRead: return os << "ParserState::StructVariableBlockCaseValueRead";
        case ParserState::StructVariableBlockCaseValueColonRead: return os << "ParserState::StructVariableBlockCaseValueColonRead";
        case ParserState::StructVariableBlockCaseBlockNameRead: return os << "ParserState::StructVariableBlockCaseBlockNameRead";
        case ParserState::StructVariableBlockNamespaceFirstColonRead: return os << "ParserState::StructVariableBlockNamespaceFirstColonRead";
        case ParserState::StructVariableBlockNamespaceSecondColonRead: return os << "ParserState::StructVariableBlockNamespaceSecondColonRead";

        default: return os << "Unknown ParserState Value";
        };
    }
}}
