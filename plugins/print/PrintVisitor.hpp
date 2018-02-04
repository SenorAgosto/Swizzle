#pragma once
#include <swizzle/ast/VisitorInterface.hpp>
#include <swizzle/ast/Node.hpp>

#include <cstdint>
#include <functional>
#include <vector>

namespace swizzle { namespace lexer {
    class FileInfo;
}}

namespace swizzle { namespace plugins { namespace print {

    class PrintVisitor : public ast::VisitorInterface
    {
    public:
        void operator()(ast::AncestorInfo& ancestors, ast::Node& node) override;

        void operator()(ast::AncestorInfo& ancestors, ast::nodes::Attribute& node) override;
        void operator()(ast::AncestorInfo& ancestors, ast::nodes::AttributeBlock& node) override;
        void operator()(ast::AncestorInfo& ancestors, ast::nodes::Bitfield& node) override;
        void operator()(ast::AncestorInfo& ancestors, ast::nodes::BitfieldField& node) override;
        void operator()(ast::AncestorInfo& ancestors, ast::nodes::CharLiteral& node) override;
        void operator()(ast::AncestorInfo& ancestors, ast::nodes::Comment& node) override;
        void operator()(ast::AncestorInfo& ancestors, ast::nodes::DefaultFloatValue& node) override;
        void operator()(ast::AncestorInfo& ancestors, ast::nodes::DefaultStringValue& node) override;
        void operator()(ast::AncestorInfo& ancestors, ast::nodes::DefaultValue& node) override;
        void operator()(ast::AncestorInfo& ancestors, ast::nodes::Enum& node) override;
        void operator()(ast::AncestorInfo& ancestors, ast::nodes::EnumField& node) override;
        void operator()(ast::AncestorInfo& ancestors, ast::nodes::Extern& node) override;
        void operator()(ast::AncestorInfo& ancestors, ast::nodes::FieldLabel& node) override;
        void operator()(ast::AncestorInfo& ancestors, ast::nodes::HexLiteral& node) override;
        void operator()(ast::AncestorInfo& ancestors, ast::nodes::Import& node) override;
        void operator()(ast::AncestorInfo& ancestors, ast::nodes::MultilineComment& node) override;
        void operator()(ast::AncestorInfo& ancestors, ast::nodes::Namespace& node) override;
        void operator()(ast::AncestorInfo& ancestors, ast::nodes::NumericLiteral& node) override;
        void operator()(ast::AncestorInfo& ancestors, ast::nodes::Root& node) override;
        void operator()(ast::AncestorInfo& ancestors, ast::nodes::StringLiteral& node) override;
        void operator()(ast::AncestorInfo& ancestors, ast::nodes::Struct& node) override;
        void operator()(ast::AncestorInfo& ancestors, ast::nodes::StructField& node) override;
        void operator()(ast::AncestorInfo& ancestors, ast::nodes::TypeAlias& node) override;
        void operator()(ast::AncestorInfo& ancestors, ast::nodes::VariableBlock& node) override;
        void operator()(ast::AncestorInfo& ancestors, ast::nodes::VariableBlockCase& node) override;
        
    protected:
        std::size_t adjust_indentation(const ast::AncestorInfo& ancestors);
        
        std::string print_node(const std::string& name, const lexer::FileInfo& info);
        std::string print_filename(const lexer::FileInfo& info);
        
        void print_multiline(const std::size_t indent, const std::string& comment);
        void print_line(const std::size_t indent, const std::string& str, char const * const bullet = "- ") const;
    };
}}}

