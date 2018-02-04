#pragma once 
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast {

    // a visitor that implements the interface with a default
    // function so we don't have to overload every method each time.
    class DefaultVisitor : public VisitorInterface
    {
    public:
        void operator()(AncestorInfo&, Node&) override {}

        void operator()(AncestorInfo&, nodes::Attribute&) override {}
        void operator()(AncestorInfo&, nodes::AttributeBlock&) override {}
        void operator()(AncestorInfo&, nodes::Bitfield&) override {}
        void operator()(AncestorInfo&, nodes::BitfieldField&) override {}
        void operator()(AncestorInfo&, nodes::CharLiteral&) override {}
        void operator()(AncestorInfo&, nodes::Comment&) override {}
        void operator()(AncestorInfo&, nodes::DefaultFloatValue&) override {}
        void operator()(AncestorInfo&, nodes::DefaultStringValue&) override {}
        void operator()(AncestorInfo&, nodes::DefaultValue&) override {}
        void operator()(AncestorInfo&, nodes::Enum&) override {}
        void operator()(AncestorInfo&, nodes::EnumField&) override {}
        void operator()(AncestorInfo&, nodes::Extern&) override {}
        void operator()(AncestorInfo&, nodes::FieldLabel&) override {}
        void operator()(AncestorInfo&, nodes::HexLiteral&) override {}
        void operator()(AncestorInfo&, nodes::Import&) override {}
        void operator()(AncestorInfo&, nodes::MultilineComment&) override {}
        void operator()(AncestorInfo&, nodes::Namespace&) override {}
        void operator()(AncestorInfo&, nodes::NumericLiteral&) override {}
        void operator()(AncestorInfo&, nodes::Root&) override {}
        void operator()(AncestorInfo&, nodes::StringLiteral&) override {}
        void operator()(AncestorInfo&, nodes::Struct&) override {}
        void operator()(AncestorInfo&, nodes::StructField&) override {}
        void operator()(AncestorInfo&, nodes::TypeAlias&) override {}
        void operator()(AncestorInfo&, nodes::VariableBlock&) override {}
        void operator()(AncestorInfo&, nodes::VariableBlockCase&) override {}
    };
}}
