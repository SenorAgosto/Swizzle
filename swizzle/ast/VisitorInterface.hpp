#pragma once 

namespace swizzle { namespace ast {
    class AncestorInfo;
    class Node;
}}

namespace swizzle { namespace ast { namespace nodes {
    class Attribute;
    class AttributeBlock;
    class Bitfield;
    class BitfieldField;
    class CharLiteral;
    class Comment;
    class DefaultFloatValue;
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
        
        virtual void operator()(AncestorInfo& ancestors, Node& node) = 0;

        virtual void operator()(AncestorInfo& ancestors, nodes::Attribute& node) = 0;
        virtual void operator()(AncestorInfo& ancestors, nodes::AttributeBlock& node) = 0;
        virtual void operator()(AncestorInfo& ancestors, nodes::Bitfield& node) = 0;
        virtual void operator()(AncestorInfo& ancestors, nodes::BitfieldField& node) = 0;
        virtual void operator()(AncestorInfo& ancestors, nodes::CharLiteral& node) = 0;
        virtual void operator()(AncestorInfo& ancestors, nodes::Comment& node) = 0;
        virtual void operator()(AncestorInfo& ancestors, nodes::DefaultFloatValue& node) = 0;
        virtual void operator()(AncestorInfo& ancestors, nodes::DefaultStringValue& node) = 0;
        virtual void operator()(AncestorInfo& ancestors, nodes::DefaultValue& node) = 0;
        virtual void operator()(AncestorInfo& ancestors, nodes::Enum& node) = 0;
        virtual void operator()(AncestorInfo& ancestors, nodes::EnumField& node) = 0;
        virtual void operator()(AncestorInfo& ancestors, nodes::Extern& node) = 0;
        virtual void operator()(AncestorInfo& ancestors, nodes::FieldLabel& node) = 0;
        virtual void operator()(AncestorInfo& ancestors, nodes::HexLiteral& node) = 0;
        virtual void operator()(AncestorInfo& ancestors, nodes::Import& node) = 0;
        virtual void operator()(AncestorInfo& ancestors, nodes::MultilineComment& node) = 0;
        virtual void operator()(AncestorInfo& ancestors, nodes::Namespace& node) = 0;
        virtual void operator()(AncestorInfo& ancestors, nodes::NumericLiteral& node) = 0;
        virtual void operator()(AncestorInfo& ancestors, nodes::Root& node) = 0;
        virtual void operator()(AncestorInfo& ancestors, nodes::StringLiteral& node) = 0;
        virtual void operator()(AncestorInfo& ancestors, nodes::Struct& node) = 0;
        virtual void operator()(AncestorInfo& ancestors, nodes::StructField& node) = 0;
        virtual void operator()(AncestorInfo& ancestors, nodes::TypeAlias& node) = 0;
        virtual void operator()(AncestorInfo& ancestors, nodes::VariableBlock& node) = 0;
        virtual void operator()(AncestorInfo& ancestors, nodes::VariableBlockCase& node) = 0;
    };
}}
