#pragma once
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace plugins { namespace print {

    class PrintVisitor : public ast::VisitorInterface
    {
    public:
        void operator()(ast::Node& parent, ast::Node& node) override;

        void operator()(ast::Node& parent, ast::nodes::Attribute& node) override;
        void operator()(ast::Node& parent, ast::nodes::AttributeBlock& node) override;
        void operator()(ast::Node& parent, ast::nodes::Bitfield& node) override;
        void operator()(ast::Node& parent, ast::nodes::BitfieldField& node) override;
        void operator()(ast::Node& parent, ast::nodes::CharLiteral& node) override;
        void operator()(ast::Node& parent, ast::nodes::Comment& node) override;
        void operator()(ast::Node& parent, ast::nodes::DefaultStringValue& node) override;
        void operator()(ast::Node& parent, ast::nodes::DefaultValue& node) override;
        void operator()(ast::Node& parent, ast::nodes::Enum& node) override;
        void operator()(ast::Node& parent, ast::nodes::EnumField& node) override;
        void operator()(ast::Node& parent, ast::nodes::Extern& node) override;
        void operator()(ast::Node& parent, ast::nodes::FieldLabel& node) override;
        void operator()(ast::Node& parent, ast::nodes::HexLiteral& node) override;
        void operator()(ast::Node& parent, ast::nodes::Import& node) override;
        void operator()(ast::Node& parent, ast::nodes::MultilineComment& node) override;
        void operator()(ast::Node& parent, ast::nodes::Namespace& node) override;
        void operator()(ast::Node& parent, ast::nodes::NumericLiteral& node) override;
        void operator()(ast::Node& parent, ast::nodes::StringLiteral& node) override;
        void operator()(ast::Node& parent, ast::nodes::Struct& node) override;
        void operator()(ast::Node& parent, ast::nodes::StructField& node) override;
        void operator()(ast::Node& parent, ast::nodes::TypeAlias& node) override;
        void operator()(ast::Node& parent, ast::nodes::VariableBlock& node) override;
        void operator()(ast::Node& parent, ast::nodes::VariableBlockCase& node) override;
    };
}}}

