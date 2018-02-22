#pragma once
#include <swizzle/ast/DefaultVisitor.hpp>
#include <string>

namespace swizzle { namespace ast { namespace matchers {

    // HasChildOf uses this visitor to determine if the matching node's
    // value matches a supplied string.
    class HasChildOfValueVisitor : public DefaultVisitor
    {
    public:
        HasChildOfValueVisitor(const std::string& value);
        
        // returns true if value_ matches the name of the node we're visiting.
        bool value_matches() const { return matches_; }
        
    public:
        void operator()(AncestorInfo&, nodes::Attribute&) override;
        void operator()(AncestorInfo&, nodes::BaseClass&) override;
        void operator()(AncestorInfo&, nodes::Bitfield&) override;
        void operator()(AncestorInfo&, nodes::BitfieldField&) override;
        void operator()(AncestorInfo&, nodes::CharLiteral&) override;
        void operator()(AncestorInfo&, nodes::DefaultFloatValue&) override;
        void operator()(AncestorInfo&, nodes::DefaultStringValue&) override;
        void operator()(AncestorInfo&, nodes::DefaultValue&) override;
        void operator()(AncestorInfo&, nodes::Enum&) override;
        void operator()(AncestorInfo&, nodes::EnumField&) override;
        void operator()(AncestorInfo&, nodes::Extern&) override;
        void operator()(AncestorInfo&, nodes::FieldLabel&) override;
        void operator()(AncestorInfo&, nodes::HexLiteral&) override;
        void operator()(AncestorInfo&, nodes::Import&) override;
        void operator()(AncestorInfo&, nodes::Namespace&) override;
        void operator()(AncestorInfo&, nodes::NumericLiteral&) override;
        void operator()(AncestorInfo&, nodes::StringLiteral&) override;
        void operator()(AncestorInfo&, nodes::Struct&) override;
        void operator()(AncestorInfo&, nodes::StructField&) override;
        void operator()(AncestorInfo&, nodes::TypeAlias&) override;
        void operator()(AncestorInfo&, nodes::VariableBlock&) override;
        void operator()(AncestorInfo&, nodes::VariableBlockCase&) override;
        
    private:
        const std::string value_;
        bool matches_ = false;
    };
}}}
