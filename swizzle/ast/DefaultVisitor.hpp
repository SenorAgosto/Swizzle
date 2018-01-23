#pragma once 
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast {

    // a visitor that implements the interface with a default
    // function so we don't have to overload every method each time.
    class DefaultVisitor : public VisitorInterface
    {
    public:
        void operator()(Node&, Node&) override {}

        void operator()(Node&, nodes::Attribute&) override {}
        void operator()(Node&, nodes::AttributeBlock&) override {}
        void operator()(Node&, nodes::Bitfield&) override {}
        void operator()(Node&, nodes::BitfieldField&) override {}
        void operator()(Node&, nodes::CharLiteral&) override {}
        void operator()(Node&, nodes::Comment&) override {}
        void operator()(Node&, nodes::DefaultStringValue&) override {}
        void operator()(Node&, nodes::DefaultValue&) override {}
        void operator()(Node&, nodes::Enum&) override {}
        void operator()(Node&, nodes::EnumField&) override {}
        void operator()(Node&, nodes::Extern&) override {}
        void operator()(Node&, nodes::FieldLabel&) override {}
        void operator()(Node&, nodes::HexLiteral&) override {}
        void operator()(Node&, nodes::Import&) override {}
        void operator()(Node&, nodes::MultilineComment&) override {}
        void operator()(Node&, nodes::Namespace&) override {}
        void operator()(Node&, nodes::NumericLiteral&) override {}
        void operator()(Node&, nodes::Root&) override {}
        void operator()(Node&, nodes::StringLiteral&) override {}
        void operator()(Node&, nodes::Struct&) override {}
        void operator()(Node&, nodes::StructField&) override {}
        void operator()(Node&, nodes::TypeAlias&) override {}
        void operator()(Node&, nodes::VariableBlock&) override {}
        void operator()(Node&, nodes::VariableBlockCase&) override {}
    };
}}
