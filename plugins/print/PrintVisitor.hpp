#pragma once
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace plugins { namespace print {

    class PrintVisitor : public ast::VisitorInterface
    {
    public:
        void operator()(ast::Node& node) override;

        void operator()(ast::nodes::Attribute& node) override;
        void operator()(ast::nodes::AttributeBlock& node) override;
        void operator()(ast::nodes::Bitfield& node) override;
        void operator()(ast::nodes::BitfieldField& node) override;
        void operator()(ast::nodes::CharLiteral& node) override;
        void operator()(ast::nodes::Comment& node) override;
        void operator()(ast::nodes::DefaultStringValue& node) override;
        void operator()(ast::nodes::DefaultValue& node) override;
        void operator()(ast::nodes::Enum& node) override;
        void operator()(ast::nodes::EnumField& node) override;
        void operator()(ast::nodes::Extern& node) override;
        void operator()(ast::nodes::FieldLabel& node) override;
        void operator()(ast::nodes::HexLiteral& node) override;
        void operator()(ast::nodes::Import& node) override;
        void operator()(ast::nodes::MultilineComment& node) override;
        void operator()(ast::nodes::Namespace& node) override;
        void operator()(ast::nodes::NumericLiteral& node) override;
        void operator()(ast::nodes::StringLiteral& node) override;
        void operator()(ast::nodes::Struct& node) override;
        void operator()(ast::nodes::StructField& node) override;
        void operator()(ast::nodes::TypeAlias& node) override;
        void operator()(ast::nodes::VariableBlock& node) override;
        void operator()(ast::nodes::VariableBlockCase& node) override;
    };
}}}

