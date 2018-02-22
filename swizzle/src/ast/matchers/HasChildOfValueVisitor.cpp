#include <swizzle/ast/matchers/HasChildOfValueVisitor.hpp>

#include <swizzle/ast/nodes/Attribute.hpp>
#include <swizzle/ast/nodes/BaseClass.hpp>
#include <swizzle/ast/nodes/Bitfield.hpp>
#include <swizzle/ast/nodes/BitfieldField.hpp>
#include <swizzle/ast/nodes/CharLiteral.hpp>
#include <swizzle/ast/nodes/DefaultFloatValue.hpp>
#include <swizzle/ast/nodes/DefaultValue.hpp>
#include <swizzle/ast/nodes/DefaultStringValue.hpp>
#include <swizzle/ast/nodes/Enum.hpp>
#include <swizzle/ast/nodes/EnumField.hpp>
#include <swizzle/ast/nodes/Extern.hpp>
#include <swizzle/ast/nodes/FieldLabel.hpp>
#include <swizzle/ast/nodes/HexLiteral.hpp>
#include <swizzle/ast/nodes/Import.hpp>
#include <swizzle/ast/nodes/Namespace.hpp>
#include <swizzle/ast/nodes/NumericLiteral.hpp>
#include <swizzle/ast/nodes/StringLiteral.hpp>
#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/ast/nodes/TypeAlias.hpp>
#include <swizzle/ast/nodes/VariableBlock.hpp>
#include <swizzle/ast/nodes/VariableBlockCase.hpp>

namespace swizzle { namespace ast { namespace matchers {

    HasChildOfValueVisitor::HasChildOfValueVisitor(const std::string& value)
        : value_(value)
    {
    }

    void HasChildOfValueVisitor::operator()(AncestorInfo&, nodes::Attribute& attribute)
    {
        matches_ = attribute.info().token().value() == value_;
    }
    
    void HasChildOfValueVisitor::operator()(AncestorInfo&, nodes::BaseClass& baseClass)
    {
        matches_ = baseClass.name() == value_;
    }
    
    void HasChildOfValueVisitor::operator()(AncestorInfo&, nodes::Bitfield& bitfield)
    {
        matches_ = bitfield.name() == value_;
    }
    
    void HasChildOfValueVisitor::operator()(AncestorInfo&, nodes::BitfieldField& bitfield_field)
    {
        matches_ = bitfield_field.name().token().value() == value_;
    }
    
    void HasChildOfValueVisitor::operator()(AncestorInfo&, nodes::CharLiteral& char_literal)
    {
        matches_ = char_literal.info().token().value() == value_;
    }
    
    void HasChildOfValueVisitor::operator()(AncestorInfo&, nodes::DefaultFloatValue& float_value)
    {
        matches_ = float_value.value().token().value() == value_;
    }
    
    void HasChildOfValueVisitor::operator()(AncestorInfo&, nodes::DefaultStringValue& string_value)
    {
        matches_ = string_value.value().token().value() == value_;
    }
    
    void HasChildOfValueVisitor::operator()(AncestorInfo&, nodes::DefaultValue& value)
    {
        matches_ = value.value().token().value() == value_;
    }
    
    void HasChildOfValueVisitor::operator()(AncestorInfo&, nodes::Enum& e)
    {
        matches_ = e.name() == value_;
    }
    
    void HasChildOfValueVisitor::operator()(AncestorInfo&, nodes::EnumField& enum_field)
    {
        matches_ = enum_field.name().token().value() == value_;
    }
    
    void HasChildOfValueVisitor::operator()(AncestorInfo&, nodes::Extern& ext)
    {
        matches_ = ext.externType().token().value() == value_;
    }
    
    void HasChildOfValueVisitor::operator()(AncestorInfo&, nodes::FieldLabel& field_label)
    {
        matches_ = field_label.label().token().value() == value_;
    }
    
    void HasChildOfValueVisitor::operator()(AncestorInfo&, nodes::HexLiteral& hex_literal)
    {
        matches_ = hex_literal.info().token().value() == value_;
    }
    
    void HasChildOfValueVisitor::operator()(AncestorInfo&, nodes::Import& import)
    {
        matches_ = import.info().token().value() == value_;
    }
    
    void HasChildOfValueVisitor::operator()(AncestorInfo&, nodes::Namespace& ns)
    {
        matches_ = ns.name().token().value() == value_;
    }
    
    void HasChildOfValueVisitor::operator()(AncestorInfo&, nodes::NumericLiteral& num_literal)
    {
        matches_ = num_literal.info().token().value() == value_;
    }
    
    void HasChildOfValueVisitor::operator()(AncestorInfo&, nodes::StringLiteral& string_literal)
    {
        matches_ = string_literal.info().token().value() == value_;
    }
    
    void HasChildOfValueVisitor::operator()(AncestorInfo&, nodes::Struct& str)
    {
        matches_ = str.name() == value_;
    }
    
    void HasChildOfValueVisitor::operator()(AncestorInfo&, nodes::StructField& struct_field)
    {
        matches_ = struct_field.name().token().value() == value_;
    }
    
    void HasChildOfValueVisitor::operator()(AncestorInfo&, nodes::TypeAlias& type_alias)
    {
        matches_ = (type_alias.aliasedType().token().value() == value_) ||
            (type_alias.existingType().token().value() == value_);
    }
    
    void HasChildOfValueVisitor::operator()(AncestorInfo&, nodes::VariableBlock& var_block)
    {
        matches_ = var_block.variableOnField().token().value() == value_;
    }
    
    void HasChildOfValueVisitor::operator()(AncestorInfo&, nodes::VariableBlockCase& var_block_case)
    {
        matches_ = (var_block_case.value().token().value() == value_) ||
            (var_block_case.type().token().value() == value_);
    }
}}}

