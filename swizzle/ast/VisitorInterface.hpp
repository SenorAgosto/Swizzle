#pragma once 

namespace swizzle { namespace ast { namespace nodes {
    class Attribute;
    class AttributeBlock;
    class CharLiteral;
    class Comment;
    class Enum;
    class EnumField;
    class HexLiteral;
    class Import;
    class MultilineComment;
    class NumericLiteral;
    class StringLiteral;
    class TypeAlias;
}}}

namespace swizzle { namespace ast {

    class VisitorInterface
    {
    public:
        virtual ~VisitorInterface(){}
        virtual void operator()(Node& node) = 0;

        virtual void operator()(nodes::Attribute& node) = 0;
        virtual void operator()(nodes::AttributeBlock& node) = 0;
        virtual void operator()(nodes::CharLiteral& node) = 0;
        virtual void operator()(nodes::Comment& node) = 0;
        virtual void operator()(nodes::Enum& node) = 0;
        virtual void operator()(nodes::EnumField& node) = 0;
        virtual void operator()(nodes::HexLiteral& node) = 0;
        virtual void operator()(nodes::Import& node) = 0;
        virtual void operator()(nodes::MultilineComment& node) = 0;
        virtual void operator()(nodes::NumericLiteral& node) = 0;
        virtual void operator()(nodes::StringLiteral& node) = 0;
        virtual void operator()(nodes::TypeAlias& node) = 0;
    };
}}
