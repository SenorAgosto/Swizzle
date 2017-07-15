#pragma once 

#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserState.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

#include <swizzle/parser/states/BitfieldColonReadState.hpp>
#include <swizzle/parser/states/BitfieldEndPositionState.hpp>
#include <swizzle/parser/states/BitfieldFieldColonReadState.hpp>
#include <swizzle/parser/states/BitfieldFieldState.hpp>
#include <swizzle/parser/states/BitfieldFirstDotState.hpp>
#include <swizzle/parser/states/BitfieldNameState.hpp>
#include <swizzle/parser/states/BitfieldSecondDotState.hpp>
#include <swizzle/parser/states/BitfieldStartPositionState.hpp>
#include <swizzle/parser/states/BitfieldStartScopeState.hpp>
#include <swizzle/parser/states/BitfieldUnderlyingTypeState.hpp>
#include <swizzle/parser/states/EnumColonReadState.hpp>
#include <swizzle/parser/states/EnumFieldEqualReadState.hpp>
#include <swizzle/parser/states/EnumFieldState.hpp>
#include <swizzle/parser/states/EnumFieldValueReadState.hpp>
#include <swizzle/parser/states/EnumNameState.hpp>
#include <swizzle/parser/states/EnumStartScopeState.hpp>
#include <swizzle/parser/states/EnumUnderlyingTypeState.hpp>
#include <swizzle/parser/states/ExternFirstColonState.hpp>
#include <swizzle/parser/states/ExternValueState.hpp>
#include <swizzle/parser/states/ImportFirstColonState.hpp>
#include <swizzle/parser/states/ImportValueState.hpp>
#include <swizzle/parser/states/InitState.hpp>
#include <swizzle/parser/states/NamespaceFirstColonState.hpp>
#include <swizzle/parser/states/NamespaceValueState.hpp>
#include <swizzle/parser/states/StartBitfieldState.hpp>
#include <swizzle/parser/states/StartEnumState.hpp>
#include <swizzle/parser/states/StartExternState.hpp>
#include <swizzle/parser/states/StartImportState.hpp>
#include <swizzle/parser/states/StartNamespaceState.hpp>
#include <swizzle/parser/states/StartStructState.hpp>
#include <swizzle/parser/states/StartUsingState.hpp>
#include <swizzle/parser/states/StructArrayState.hpp>
#include <swizzle/parser/states/StructFieldLabelState.hpp>
#include <swizzle/parser/states/StructFieldNameState.hpp>
#include <swizzle/parser/states/StructFieldNamespaceFirstColonState.hpp>
#include <swizzle/parser/states/StructFieldNamespaceOrTypeState.hpp>
#include <swizzle/parser/states/StructFieldNamespaceSecondColonState.hpp>
#include <swizzle/parser/states/StructNameState.hpp>
#include <swizzle/parser/states/StructStartArrayState.hpp>
#include <swizzle/parser/states/StructStartScopeState.hpp>
#include <swizzle/parser/states/StructStartVariableBlockState.hpp>
#include <swizzle/parser/states/StructVariableBlockBeginCasesState.hpp>
#include <swizzle/parser/states/StructVariableBlockCaseBlockNameReadState.hpp>
#include <swizzle/parser/states/StructVariableBlockCaseValueColonReadState.hpp>
#include <swizzle/parser/states/StructVariableBlockCaseValueReadState.hpp>
#include <swizzle/parser/states/StructVariableBlockCaseValueState.hpp>
#include <swizzle/parser/states/StructVariableBlockColonReadState.hpp>
#include <swizzle/parser/states/StructVariableBlockNamespaceFirstColonReadState.hpp>
#include <swizzle/parser/states/StructVariableBlockNamespaceSecondColonReadState.hpp>
#include <swizzle/parser/states/StructVariableBlockOnFieldState.hpp>
#include <swizzle/parser/states/StructVariableBlockOnNestedFieldState.hpp>
#include <swizzle/parser/states/StructVectorNestedOnMemberState.hpp>
#include <swizzle/parser/states/StructVectorState.hpp>
#include <swizzle/parser/states/TranslationUnitMainState.hpp>
#include <swizzle/parser/states/UsingFirstColonState.hpp>
#include <swizzle/parser/states/UsingNameState.hpp>
#include <swizzle/parser/states/UsingReadEqualState.hpp>
#include <swizzle/parser/states/UsingSecondColonState.hpp>
#include <swizzle/parser/states/UsingTypeReadState.hpp>

namespace swizzle { namespace parser {

    class ParserStatesPack
    {
    public:

        ParserState consume(const ParserState state, const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack& tokenStack, ParserStateContext& context)
        {
            switch(state)
            {
            case ParserState::Init: return initState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StartNamespace: return startNamespaceState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::NamespaceValue: return namespaceValueState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::NamespaceFirstColon: return namespaceFirstColonState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::TranslationUnitMain: return translationUnitMainState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StartImport: return startImportState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::ImportValue: return importValueState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::ImportFirstColon: return importFirstColonState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StartExtern: return startExternState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::ExternValue: return externValueState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::ExternFirstColon: return externFirstColonState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StartUsing: return startUsingState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::UsingName: return usingNameState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::UsingReadEqual: return usingReadEqualState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::UsingTypeRead: return usingTypeReadState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::UsingFirstColon: return usingFirstColonState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::UsingSecondColon: return usingSecondColonState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StartEnum: return startEnumState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::EnumName: return enumNameState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::EnumColonRead: return enumColonReadState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::EnumUnderlyingType: return enumUnderlyingTypeState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::EnumStartScope: return enumStartScopeState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::EnumField: return enumFieldState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::EnumFieldEqualRead: return enumFieldEualReadState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::EnumFieldValueRead: return enumFieldValueReadState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StartBitfield: return startBitfieldState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::BitfieldName: return bitfieldNameState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::BitfieldColonRead: return bitfieldColonReadState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::BitfieldUnderlyingType: return bitfieldUnderlyingTypeState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::BitfieldStartScope: return bitfieldStartScopeState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::BitfieldField: return bitfieldFieldState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::BitfieldFieldColonRead: return bitfieldFieldColonReadState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::BitfieldStartPosition: return bitfieldStartPositionState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::BitfieldFirstDot: return bitfieldFirstDotState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::BitfieldSecondDot: return bitfieldSecondDotState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::BitfieldEndPosition: return bitfieldEndPositionState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StartStruct: return startStructState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StructName: return structNameState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StructStartScope: return structStartScopeState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StructFieldLabel: return structFieldLabelState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StructFieldNamespaceOrType: return structFieldNamespaceOrTypeState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StructFieldNamespaceFirstColon: return structFieldNamespaceFirstColonState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StructFieldNamespaceSecondColon: return structFieldNamespaceSecondColonState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StructFieldName: return structFieldNameState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StructStartArray: return structStartArrayState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StructArray: return structArrayState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StructVector: return structVectorState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StructVectorNestedOnMember: return structVectorNestedOnMemberState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StructStartVariableBlock: return structStartVariableBlockState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StructVariableBlockColonRead: return structVariableBlockColonReadState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StructVariableBlockOnField: return structVariableBlockOnFieldState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StructVariableBlockOnNestedField: return structVariableBlockOnNestedFieldState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StructVariableBlockBeginCases: return structVariableBlockBeginCasesState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StructVariableBlockCaseValue: return structVariableBlockCaseValueState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StructVariableBlockCaseValueRead: return structVariableBlockCaseValueReadState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StructVariableBlockCaseValueColonRead: return structVariableBlockCaseValueColonReadState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StructVariableBlockCaseBlockNameRead: return structVariableBlockCaseBlockNameReadState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StructVariableBlockNamespaceFirstColonRead: return structVariableBlockNamespaceFirstColonReadState_.consume(token, nodeStack, tokenStack, context);
            case ParserState::StructVariableBlockNamespaceSecondColonRead: return structVariableBlockNamespaceSecondColonReadState_.consume(token, nodeStack, tokenStack, context);
            };
        }

    private:

        states::InitState initState_;
        states::StartNamespaceState startNamespaceState_;
        states::NamespaceValueState namespaceValueState_;
        states::NamespaceFirstColonState namespaceFirstColonState_;
        states::TranslationUnitMainState translationUnitMainState_;
        states::StartImportState startImportState_;
        states::ImportValueState importValueState_;
        states::ImportFirstColonState importFirstColonState_;
        states::StartExternState startExternState_;
        states::ExternValueState externValueState_;
        states::ExternFirstColonState externFirstColonState_;
        states::StartUsingState startUsingState_;
        states::UsingNameState usingNameState_;
        states::UsingReadEqualState usingReadEqualState_;
        states::UsingTypeReadState usingTypeReadState_;
        states::UsingFirstColonState usingFirstColonState_;
        states::UsingSecondColonState usingSecondColonState_;
        states::StartEnumState startEnumState_;
        states::EnumNameState enumNameState_;
        states::EnumColonReadState enumColonReadState_;
        states::EnumUnderlyingTypeState enumUnderlyingTypeState_;
        states::EnumStartScopeState enumStartScopeState_;
        states::EnumFieldState enumFieldState_;
        states::EnumFieldEqualReadState enumFieldEualReadState_;
        states::EnumFieldValueReadState enumFieldValueReadState_;
        states::StartBitfieldState startBitfieldState_;
        states::BitfieldNameState bitfieldNameState_;
        states::BitfieldColonReadState bitfieldColonReadState_;
        states::BitfieldUnderlyingTypeState bitfieldUnderlyingTypeState_;
        states::BitfieldStartScopeState bitfieldStartScopeState_;
        states::BitfieldFieldState bitfieldFieldState_;
        states::BitfieldFieldColonReadState bitfieldFieldColonReadState_;
        states::BitfieldStartPositionState bitfieldStartPositionState_;
        states::BitfieldFirstDotState bitfieldFirstDotState_;
        states::BitfieldSecondDotState bitfieldSecondDotState_;
        states::BitfieldEndPositionState bitfieldEndPositionState_;
        states::StartStructState startStructState_;
        states::StructNameState structNameState_;
        states::StructStartScopeState structStartScopeState_;
        states::StructFieldLabelState structFieldLabelState_;
        states::StructFieldNamespaceOrTypeState structFieldNamespaceOrTypeState_;
        states::StructFieldNamespaceFirstColonState structFieldNamespaceFirstColonState_;
        states::StructFieldNamespaceSecondColonState structFieldNamespaceSecondColonState_;
        states::StructFieldNameState structFieldNameState_;
        states::StructStartArrayState structStartArrayState_;
        states::StructArrayState structArrayState_;
        states::StructVectorState structVectorState_;
        states::StructVectorNestedOnMemberState structVectorNestedOnMemberState_;
        states::StructStartVariableBlockState structStartVariableBlockState_;
        states::StructVariableBlockColonReadState structVariableBlockColonReadState_;
        states::StructVariableBlockOnFieldState structVariableBlockOnFieldState_;
        states::StructVariableBlockOnNestedFieldState structVariableBlockOnNestedFieldState_;
        states::StructVariableBlockBeginCasesState structVariableBlockBeginCasesState_;
        states::StructVariableBlockCaseValueState structVariableBlockCaseValueState_;
        states::StructVariableBlockCaseValueReadState structVariableBlockCaseValueReadState_;
        states::StructVariableBlockCaseValueColonReadState structVariableBlockCaseValueColonReadState_;
        states::StructVariableBlockCaseBlockNameReadState structVariableBlockCaseBlockNameReadState_;
        states::StructVariableBlockNamespaceFirstColonReadState structVariableBlockNamespaceFirstColonReadState_;
        states::StructVariableBlockNamespaceSecondColonReadState structVariableBlockNamespaceSecondColonReadState_;
    };
}}
