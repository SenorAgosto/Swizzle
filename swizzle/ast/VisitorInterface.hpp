#pragma once 

namespace swizzle { namespace ast { namespace nodes {
    class Attribute;
    class AttributeBlock;
    class Bitfield;
    class BitfieldField;
    class CharLiteral;
    class Comment;
    class DefaultValue;
    class Enum;
    class EnumField;
    class FieldLabel;
    class HexLiteral;
    class Import;
    class MultilineComment;
    class Namespace;
    class NumericLiteral;
    class StringLiteral;
    class Struct;
    class StructField;
    class TypeAlias;
    class VariableBlock;
    class VariableBlockCase;
}}}

namespace swizzle { namespace ast {

    class VisitorInterface
    {
    public:
        virtual ~VisitorInterface(){}
        virtual void operator()(Node& node) = 0;

        virtual void operator()(nodes::Attribute& node) = 0;
        virtual void operator()(nodes::AttributeBlock& node) = 0;
        virtual void operator()(nodes::Bitfield& node) = 0;
        virtual void operator()(nodes::BitfieldField& node) = 0;
        virtual void operator()(nodes::CharLiteral& node) = 0;
        virtual void operator()(nodes::Comment& node) = 0;
        virtual void operator()(nodes::DefaultValue& node) = 0;
        virtual void operator()(nodes::Enum& node) = 0;
        virtual void operator()(nodes::EnumField& node) = 0;
        virtual void operator()(nodes::FieldLabel& node) = 0;
        virtual void operator()(nodes::HexLiteral& node) = 0;
        virtual void operator()(nodes::Import& node) = 0;
        virtual void operator()(nodes::MultilineComment& node) = 0;
        virtual void operator()(nodes::Namespace& node) = 0;
        virtual void operator()(nodes::NumericLiteral& node) = 0;
        virtual void operator()(nodes::StringLiteral& node) = 0;
        virtual void operator()(nodes::Struct& node) = 0;
        virtual void operator()(nodes::StructField& node) = 0;
        virtual void operator()(nodes::TypeAlias& node) = 0;
        virtual void operator()(nodes::VariableBlock& node) = 0;
        virtual void operator()(nodes::VariableBlockCase& node) = 0;
    };
}}
