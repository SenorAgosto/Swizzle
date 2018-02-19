#include <swizzle/parser/ParserStatesPack.hpp>
#include <swizzle/Exceptions.hpp>

#include <sstream>

namespace swizzle { namespace parser {

    ParserState ParserStatesPack::consume(const ParserState state, const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack& attributeStack, types::TokenStack& tokenStack, ParserStateContext& context)
    {
        switch(state)
        {
        case ParserState::Init: return initState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StartNamespace: return startNamespaceState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::NamespaceValue: return namespaceValueState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::NamespaceFirstColon: return namespaceFirstColonState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::TranslationUnitMain: return translationUnitMainState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StartImport: return startImportState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::ImportValue: return importValueState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::ImportFirstColon: return importFirstColonState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StartExtern: return startExternState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::ExternValue: return externValueState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::ExternFirstColon: return externFirstColonState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StartUsing: return startUsingState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::UsingName: return usingNameState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::UsingReadEqual: return usingReadEqualState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::UsingTypeRead: return usingTypeReadState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::UsingFirstColon: return usingFirstColonState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::UsingSecondColon: return usingSecondColonState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StartEnum: return startEnumState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::EnumName: return enumNameState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::EnumColonRead: return enumColonReadState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::EnumUnderlyingType: return enumUnderlyingTypeState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::EnumStartScope: return enumStartScopeState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::EnumField: return enumFieldState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::EnumFieldEqualRead: return enumFieldEualReadState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::EnumFieldValueRead: return enumFieldValueReadState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StartBitfield: return startBitfieldState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::BitfieldName: return bitfieldNameState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::BitfieldColonRead: return bitfieldColonReadState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::BitfieldUnderlyingType: return bitfieldUnderlyingTypeState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::BitfieldStartScope: return bitfieldStartScopeState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::BitfieldField: return bitfieldFieldState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::BitfieldFieldColonRead: return bitfieldFieldColonReadState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::BitfieldStartPosition: return bitfieldStartPositionState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::BitfieldFirstDot: return bitfieldFirstDotState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::BitfieldSecondDot: return bitfieldSecondDotState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::BitfieldEndPosition: return bitfieldEndPositionState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructBaseColonRead: return structBaseColonReadState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructBaseNameOrNamespaceFirstColonRead return structBaseNameOrNamespaceReadState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructBaseNameOrNamespaceRead: return structBaseNameOrNamespaceReadState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StartStruct: return startStructState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructName: return structNameState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructStartScope: return structStartScopeState_.consume(token, nodeStack, attributeStack,  tokenStack, context);
        case ParserState::StructFieldLabel: return structFieldLabelState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructFieldEnumOrNamespaceRead: return structFieldEnumOrNamespaceReadState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructFieldEnumOrNamespaceFirstColonRead: return structFieldEnumOrNamespaceFirstColonReadState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructFieldEnumValueRead: return structFieldEnumValueReadState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructFieldNamespaceOrType: return structFieldNamespaceOrTypeState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructFieldNamespaceFirstColon: return structFieldNamespaceFirstColonState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructFieldNamespaceSecondColon: return structFieldNamespaceSecondColonState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructFieldName: return structFieldNameState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructFieldEqualRead: return structFieldEqualReadState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructFieldValueRead: return structFieldValueReadState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructStartArray: return structStartArrayState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructArray: return structArrayState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructVector: return structVectorState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructVectorNestedOnMember: return structVectorNestedOnMemberState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructEndArrayOrVector: return structEndArrayOrVectorState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructStartVariableBlock: return structStartVariableBlockState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructVariableBlockColonRead: return structVariableBlockColonReadState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructVariableBlockOnField: return structVariableBlockOnFieldState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructVariableBlockOnNestedField: return structVariableBlockOnNestedFieldState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructVariableBlockBeginCases: return structVariableBlockBeginCasesState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructVariableBlockCaseValue: return structVariableBlockCaseValueState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructVariableBlockCaseValueRead: return structVariableBlockCaseValueReadState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructVariableBlockCaseValueColonRead: return structVariableBlockCaseValueColonReadState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructVariableBlockCaseBlockNameRead: return structVariableBlockCaseBlockNameReadState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructVariableBlockNamespaceFirstColonRead: return structVariableBlockNamespaceFirstColonReadState_.consume(token, nodeStack, attributeStack, tokenStack, context);
        case ParserState::StructVariableBlockNamespaceSecondColonRead: return structVariableBlockNamespaceSecondColonReadState_.consume(token, nodeStack, attributeStack, tokenStack, context);

        default:
            break;
        };

        std::stringstream ss;
        ss << static_cast<std::size_t>(state);

        throw ParserError("Internal parser error, unrecognized parser state: " + ss.str());
    }
}}
