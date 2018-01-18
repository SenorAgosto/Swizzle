#include "./PrintVisitor.hpp"

#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Attribute.hpp>
#include <swizzle/ast/nodes/AttributeBlock.hpp>
#include <swizzle/ast/nodes/Bitfield.hpp>
#include <swizzle/ast/nodes/BitfieldField.hpp>
#include <swizzle/ast/nodes/CharLiteral.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/DefaultStringValue.hpp>
#include <swizzle/ast/nodes/DefaultValue.hpp>
#include <swizzle/ast/nodes/Enum.hpp>
#include <swizzle/ast/nodes/EnumField.hpp>
#include <swizzle/ast/nodes/Extern.hpp>
#include <swizzle/ast/nodes/FieldLabel.hpp>
#include <swizzle/ast/nodes/HexLiteral.hpp>
#include <swizzle/ast/nodes/Import.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/ast/nodes/Namespace.hpp>
#include <swizzle/ast/nodes/NumericLiteral.hpp>
#include <swizzle/ast/nodes/StringLiteral.hpp>
#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/ast/nodes/TypeAlias.hpp>
#include <swizzle/ast/nodes/VariableBlock.hpp>
#include <swizzle/ast/nodes/VariableBlockCase.hpp>

#include <iostream>

namespace swizzle { namespace plugins { namespace print {

    void PrintVisitor::operator()(ast::Node& parent, ast::Node& node)
    {
        std::cout << "- NODE" << "\n";
    }

    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::Attribute& node)
    {
        std::cout << "- ATTRIBUTE" << "\n";
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::AttributeBlock& node)
    {
        std::cout << "- ATTRIBUTE_BLOCK" << "\n";
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::Bitfield& node)
    {
        std::cout << "- BITFIELD" << "\n";
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::BitfieldField& node)
    {
        std::cout << "- BITFIELD_FIELD" << "\n";
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::CharLiteral& node)
    {
        std::cout << "- CHAR_LITERAL" << "\n";
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::Comment& node)
    {
        std::cout << "- COMMENT" << "\n";
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::DefaultStringValue& node)
    {
        std::cout << "- DEFAULT_STRING_VALUE" << "\n";
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::DefaultValue& node)
    {
        std::cout << "- DEFAULT VALUE" << "\n";
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::Enum& node)
    {
        std::cout << "- ENUM" << "\n";
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::EnumField& node)
    {
        std::cout << "- ENUM_FIELD" << "\n";
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::Extern& node)
    {
        std::cout << "- EXTERN" << "\n";
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::FieldLabel& node)
    {
        std::cout << "- FIELD_LABEL" << "\n";
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::HexLiteral& node)
    {
        std::cout << "HEX_LITERAL" << "\n";
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::Import& node)
    {
        std::cout << "IMPORT" << "\n";
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::MultilineComment& node)
    {
        std::cout << "MULTILINE_COMMENT" << "\n";
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::Namespace& node)
    {
        std::cout << "NAMESPACE" << "\n";
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::NumericLiteral& node)
    {
        std::cout << "NUMERIC_LITERAL" << "\n";
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::StringLiteral& node)
    {
        std::cout << "STRING_LITERAL" << "\n";
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::Struct& node)
    {
        std::cout << "STRUCT" << "\n";
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::StructField& node)
    {
        std::cout << "STRUCT_FIELD" << "\n";
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::TypeAlias& node)
    {
        std::cout << "TYPE_ALIAS" << "\n";
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::VariableBlock& node)
    {
        std::cout << "VARIABLE_BLOCK" << "\n";
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::VariableBlockCase& node)
    {
        std::cout << "VARIABLE_BLOCK_CASE" << "\n";
    }
}}}
