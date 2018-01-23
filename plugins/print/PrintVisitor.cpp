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
#include <swizzle/ast/nodes/Root.hpp>
#include <swizzle/ast/nodes/StringLiteral.hpp>
#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/ast/nodes/TypeAlias.hpp>
#include <swizzle/ast/nodes/VariableBlock.hpp>
#include <swizzle/ast/nodes/VariableBlockCase.hpp>

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>

namespace swizzle { namespace plugins { namespace print {
    
    PrintVisitor::AncestorContainer::iterator PrintVisitor::find(const ast::Node& node)
    {
        for(auto iter = ancestors_.begin(), end = ancestors_.end(); iter != end; ++iter)
        {
            if(&iter->get() == &node) return iter;
        }
        
        return ancestors_.end();
    }
    
    std::ptrdiff_t PrintVisitor::adjust_indentation(const ast::Node& parent, const ast::Node& node)
    {
        const auto iter = find(parent);
        const auto distance = std::distance(begin(ancestors_), iter);
        
        auto iter2 = iter;
        std::advance(iter2, 1);
        
        ancestors_.erase(iter2, end(ancestors_));
        ancestors_.push_back(node);
        
        return distance;
    }

    void PrintVisitor::print_line(const std::ptrdiff_t indent, const std::string& str) const
    {
        for(std::uint16_t i = 0; i < indent; ++i)
        {
            std::cout << "   ";
        }
        
        std::cout << "- " << str << "\n";
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::Node& node)
    {
        print_line(adjust_indentation(parent, node), "UNKNOWN NODE");
    }

    void PrintVisitor::operator()(ast::Node&, ast::nodes::Root& node)
    {
        ancestors_.push_back(node);
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::Attribute& node)
    {
        print_line(adjust_indentation(parent, node), "ATTRIBUTE");
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::AttributeBlock& node)
    {
        print_line(adjust_indentation(parent, node), "ATTRIBUTE_BLOCK");
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::Bitfield& node)
    {
        print_line(adjust_indentation(parent, node), "BITFIELD");
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::BitfieldField& node)
    {
        print_line(adjust_indentation(parent, node), "BITFIELD_FIELD");
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::CharLiteral& node)
    {
        print_line(adjust_indentation(parent, node), "CHAR_LITERAL");
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::Comment& node)
    {
        print_line(adjust_indentation(parent, node), "COMMENT");
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::DefaultStringValue& node)
    {
        print_line(adjust_indentation(parent, node), "DEFAULT_STRING_VALUE");
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::DefaultValue& node)
    {
        print_line(adjust_indentation(parent, node), "DEFAULT VALUE");
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::Enum& node)
    {
        print_line(adjust_indentation(parent, node), "ENUM");
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::EnumField& node)
    {
        print_line(adjust_indentation(parent, node), "ENUM_FIELD");
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::Extern& node)
    {
        print_line(adjust_indentation(parent, node), "EXTERN");
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::FieldLabel& node)
    {
        print_line(adjust_indentation(parent, node), "FIELD_LABEL");
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::HexLiteral& node)
    {
        print_line(adjust_indentation(parent, node), "HEX_LITERAL");
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::Import& node)
    {
        print_line(adjust_indentation(parent, node), "IMPORT");
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::MultilineComment& node)
    {
        print_line(adjust_indentation(parent, node), "MULTILINE_COMMENT");
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::Namespace& node)
    {
        print_line(adjust_indentation(parent, node), "NAMESPACE");
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::NumericLiteral& node)
    {
        print_line(adjust_indentation(parent, node), "NUMERIC_LITERAL");
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::StringLiteral& node)
    {
        print_line(adjust_indentation(parent, node), "STRING_LITERAL");
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::Struct& node)
    {
        print_line(adjust_indentation(parent, node), "STRUCT");
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::StructField& node)
    {
        print_line(adjust_indentation(parent, node), "STRUCT_FIELD");
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::TypeAlias& node)
    {
        print_line(adjust_indentation(parent, node), "TYPE_ALIAS");
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::VariableBlock& node)
    {
        print_line(adjust_indentation(parent, node), "VARIABLE_BLOCK");
    }
    
    void PrintVisitor::operator()(ast::Node& parent, ast::nodes::VariableBlockCase& node)
    {
        print_line(adjust_indentation(parent, node), "VARIABLE_BLOCK_CASE");
    }
}}}
