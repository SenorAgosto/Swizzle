#include "./PrintVisitor.hpp"

#include <swizzle/ast/AncestorInfo.hpp>
#include <swizzle/ast/Node.hpp>
#include <swizzle/ast/nodes/Attribute.hpp>
#include <swizzle/ast/nodes/AttributeBlock.hpp>
#include <swizzle/ast/nodes/BaseClass.hpp>
#include <swizzle/ast/nodes/Bitfield.hpp>
#include <swizzle/ast/nodes/BitfieldField.hpp>
#include <swizzle/ast/nodes/CharLiteral.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/DefaultFloatValue.hpp>
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
#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/types/utils/StartsWith.hpp>

#include <algorithm>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>

namespace swizzle { namespace plugins { namespace print {
    
    std::size_t PrintVisitor::adjust_indentation(const ast::AncestorInfo& ancestors)
    {
        return ancestors.depth() - 1;
    }

    std::string PrintVisitor::print_node(const std::string& name, const lexer::FileInfo& info)
    {
        return name + " <" + print_filename(info) + "> ";
    }
    
    std::string PrintVisitor::print_filename(const lexer::FileInfo& info)
    {
        // TODO: figure out how to strip . and .. directories from start of path
        std::stringstream ss;
        ss  << info.filename()
            << ":"
            << info.start().line()
            << ":"
            << info.start().column();
        
        return ss.str();
    }
    
    void PrintVisitor::print_line(const std::size_t indent, const std::string& str, char const * const bullet) const
    {
        for(std::size_t i = 0; i < indent; ++i)
        {
            std::cout << "   ";
        }
        
        std::cout << bullet << str << "\n";
    }
    
    void PrintVisitor::print_multiline(const std::size_t indent, const std::string& comment)
    {
        for(std::size_t i = 0; i < indent + 1; ++i) std::cout << "   ";

        std::vector<std::string> lines;
        boost::algorithm::split(lines, comment, boost::algorithm::is_any_of("\\"));

        if(lines.size() > 0)
        {
            std::cout << lines[0] << " \\ ...\n";
        }
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::Node&)
    {
        print_line(adjust_indentation(ancestors), "unknown node");
    }

    void PrintVisitor::operator()(ast::AncestorInfo&, ast::nodes::Root&)
    {
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::Attribute& node)
    {
        std::stringstream ss;
        ss  << print_node("attribute", node.info().fileInfo())
            << node.info().token().value();
        
        print_line(adjust_indentation(ancestors), ss.str());
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::AttributeBlock& node)
    {
        std::stringstream ss;
        ss  << print_node("attribute block", node.info().fileInfo())
            << node.info().token().value();
        
        print_line(adjust_indentation(ancestors), ss.str());
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::BaseClass& node)
    {
        std::stringstream ss;
        ss  << print_node("inherits from", node.nameDecl().fileInfo())
            << node.name();
        
        print_line(adjust_indentation(ancestors), ss.str());
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::Bitfield& node)
    {
        std::stringstream ss;
        ss  << print_node("bitfield", node.bitfieldInfo().fileInfo())
            << node.name() << " (" << node.underlying().token().value() << ")";
        
        print_line(adjust_indentation(ancestors), ss.str());
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::BitfieldField& node)
    {
        std::stringstream ss;
        ss  << print_node("bitfield field", node.name().fileInfo())
            << node.name().token().value() << " "
            << node.beginBit() << ".."
            << node.endBit();
        
        print_line(adjust_indentation(ancestors), ss.str());
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::CharLiteral& node)
    {
        std::stringstream ss;
        ss  << print_node("char literal", node.info().fileInfo())
            << node.info().token().value();
        
        print_line(adjust_indentation(ancestors), ss.str());
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::Comment& node)
    {
        std::stringstream ss;
        ss  << print_node("comment", node.info().fileInfo());
        
        print_line(adjust_indentation(ancestors), ss.str());
        print_line(adjust_indentation(ancestors), node.info().token().value().to_string(), "   ");
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::DefaultFloatValue& node)
    {
        std::stringstream ss;
        ss  << print_node("default float value", node.value().fileInfo())
            << node.value().token().value();
        
        print_line(adjust_indentation(ancestors), ss.str());
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::DefaultStringValue& node)
    {
        std::stringstream ss;
        ss  << print_node("default string value", node.value().fileInfo())
            << node.value().token().value();
        
        print_line(adjust_indentation(ancestors), ss.str());
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::DefaultValue& node)
    {
        std::stringstream ss;
        ss  << print_node("default value", node.value().fileInfo())
            << node.value().token().value();
        
        print_line(adjust_indentation(ancestors), ss.str());
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::Enum& node)
    {
        std::stringstream ss;
        ss  << print_node("enum", node.enumInfo().fileInfo())
            << node.name() << " (" << node.underlying().token().value() << ")";
        
        print_line(adjust_indentation(ancestors), ss.str());
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::EnumField& node)
    {
        std::stringstream ss;
        ss  << print_node("enum field", node.name().fileInfo())
            << node.name().token().value() << " = ";
        
        if(node.valueInfo().token().value().empty())
        {
            ss << node.value();
        }
        else
        {
            ss << node.valueInfo().token().value();
        }
        
        print_line(adjust_indentation(ancestors), ss.str());
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::Extern& node)
    {
        std::stringstream ss;
        ss  << print_node("extern", node.externType().fileInfo())
            << node.externType().token().value();
        
        print_line(adjust_indentation(ancestors), ss.str());
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::FieldLabel& node)
    {
        std::stringstream ss;
        ss  << print_node("field label", node.info().fileInfo())
            << "(" << node.info().token().value() << ")";
        
        print_line(adjust_indentation(ancestors), ss.str());
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::HexLiteral& node)
    {
        std::stringstream ss;
        ss  << print_node("hex literal", node.info().fileInfo())
            << node.info().token().value();
        
        print_line(adjust_indentation(ancestors), ss.str());
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::Import& node)
    {
        std::stringstream ss;
        ss  << print_node("import", node.info().fileInfo())
            << node.path();
        
        print_line(adjust_indentation(ancestors), ss.str());
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::MultilineComment& node)
    {
        std::stringstream ss;
        ss  << print_node("multiline comment", node.info().fileInfo());
        
        const auto indent = adjust_indentation(ancestors);
        print_line(indent, ss.str());
        print_multiline(indent, node.info().token().value().to_string());
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::Namespace& node)
    {
        std::stringstream ss;
        ss  << print_node("namespace", node.info().fileInfo())
            << node.info().token().value();
        
        print_line(adjust_indentation(ancestors), ss.str());
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::NumericLiteral& node)
    {
        std::stringstream ss;
        ss  << print_node("numeric literal", node.info().fileInfo())
            << node.info().token().value();
        
        print_line(adjust_indentation(ancestors), ss.str());
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::StringLiteral& node)
    {
        std::stringstream ss;
        ss  << print_node("string literal", node.info().fileInfo())
            << node.info().token().value();
        
        print_line(adjust_indentation(ancestors), ss.str());
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::Struct& node)
    {
        std::stringstream ss;
        ss  << print_node("struct", node.keyword().fileInfo())
            << node.name();
        
        print_line(adjust_indentation(ancestors), ss.str());
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::StructField& node)
    {
        std::stringstream ss;
        ss  << print_node("struct field", node.name().fileInfo())
            << (node.isConst() ? "const " : "")
            << node.type() << " "
            << node.name().token().value();
        
        if(node.isArray())
        {
            ss << "[" << node.arraySize() << "]";
        }
        
        if(node.isVector())
        {
            ss << "[" << node.vectorSizeMember().token().value() << "]";
        }

        print_line(adjust_indentation(ancestors), ss.str());
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::TypeAlias& node)
    {
        std::stringstream ss;
        ss  << print_node("type alias", node.existingType().fileInfo())
            << node.aliasedType().token().value() << " = "
            << node.existingType().token().value();
        
        print_line(adjust_indentation(ancestors), ss.str());
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::VariableBlock& node)
    {
        std::stringstream ss;
        ss  << print_node("variable block", node.variableBlockInfo().fileInfo())
            << "(" << node.variableOnField().token().value() << ")";
        
        print_line(adjust_indentation(ancestors), ss.str());
    }
    
    void PrintVisitor::operator()(ast::AncestorInfo& ancestors, ast::nodes::VariableBlockCase& node)
    {
        std::stringstream ss;
        ss  << print_node("variable block case", node.value().fileInfo())
            << "case " << node.value().token().value() << ": "
            << node.type().token().value();
        
        print_line(adjust_indentation(ancestors), ss.str());
    }
}}}
