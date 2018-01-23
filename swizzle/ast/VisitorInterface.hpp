#pragma once 

namespace swizzle { namespace ast {
    class Node;
}}

namespace swizzle { namespace ast { namespace nodes {
    class Attribute;
    class AttributeBlock;
    class Bitfield;
    class BitfieldField;
    class CharLiteral;
    class Comment;
    class DefaultStringValue;
    class DefaultValue;
    class Enum;
    class EnumField;
    class Extern;
    class FieldLabel;
    class HexLiteral;
    class Import;
    class MultilineComment;
    class Namespace;
    class NumericLiteral;
    class Root;
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
        
        virtual void operator()(Node& parent, Node& node) = 0;

        virtual void operator()(Node& parent, nodes::Attribute& node) = 0;
        virtual void operator()(Node& parent, nodes::AttributeBlock& node) = 0;
        virtual void operator()(Node& parent, nodes::Bitfield& node) = 0;
        virtual void operator()(Node& parent, nodes::BitfieldField& node) = 0;
        virtual void operator()(Node& parent, nodes::CharLiteral& node) = 0;
        virtual void operator()(Node& parent, nodes::Comment& node) = 0;
        virtual void operator()(Node& parent, nodes::DefaultStringValue& node) = 0;
        virtual void operator()(Node& parent, nodes::DefaultValue& node) = 0;
        virtual void operator()(Node& parent, nodes::Enum& node) = 0;
        virtual void operator()(Node& parent, nodes::EnumField& node) = 0;
        virtual void operator()(Node& parent, nodes::Extern& node) = 0;
        virtual void operator()(Node& parent, nodes::FieldLabel& node) = 0;
        virtual void operator()(Node& parent, nodes::HexLiteral& node) = 0;
        virtual void operator()(Node& parent, nodes::Import& node) = 0;
        virtual void operator()(Node& parent, nodes::MultilineComment& node) = 0;
        virtual void operator()(Node& parent, nodes::Namespace& node) = 0;
        virtual void operator()(Node& parent, nodes::NumericLiteral& node) = 0;
        virtual void operator()(Node& parent, nodes::Root& node) = 0;
        virtual void operator()(Node& parent, nodes::StringLiteral& node) = 0;
        virtual void operator()(Node& parent, nodes::Struct& node) = 0;
        virtual void operator()(Node& parent, nodes::StructField& node) = 0;
        virtual void operator()(Node& parent, nodes::TypeAlias& node) = 0;
        virtual void operator()(Node& parent, nodes::VariableBlock& node) = 0;
        virtual void operator()(Node& parent, nodes::VariableBlockCase& node) = 0;
    };
}}
