#pragma once 
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast {

    // a visitor that implements the interface with a default
    // function so we don't have to overload every method each time.
    class DefaultVisitor : public VisitorInterface
    {
    public:
        void operator()(Node&) override {}

        void operator()(nodes::Attribute&) override {}
        void operator()(nodes::AttributeBlock&) override {}
        void operator()(nodes::Bitfield&) override {}
        void operator()(nodes::BitfieldField&) override {}
        void operator()(nodes::CharLiteral&) override {}
        void operator()(nodes::Comment&) override {}
        void operator()(nodes::DefaultValue&) override {}
        void operator()(nodes::Enum&) override {}
        void operator()(nodes::EnumField&) override {}
        void operator()(nodes::FieldLabel&) override {}
        void operator()(nodes::HexLiteral&) override {}
        void operator()(nodes::Import&) override {}
        void operator()(nodes::MultilineComment&) override {}
        void operator()(nodes::Namespace&) override {}
        void operator()(nodes::NumericLiteral&) override {}
        void operator()(nodes::StringLiteral&) override {}
        void operator()(nodes::Struct&) override {}
        void operator()(nodes::StructField&) override {}
        void operator()(nodes::TypeAlias&) override {}
        void operator()(nodes::VariableBlock&) override {}
        void operator()(nodes::VariableBlockCase&) override {}
    };
}}
