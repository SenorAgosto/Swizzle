#include <swizzle/ast/nodes/EnumField.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    EnumField::EnumField(const lexer::TokenInfo& name, const lexer::TokenInfo& underlyingType)
        : name_(name)
        , underlying_(underlyingType)
    {
    }

    const lexer::TokenInfo& EnumField::name() const
    {
        return name_;
    }

    const lexer::TokenInfo& EnumField::underlying() const
    {
        return underlying_;
    }

    void EnumField::valueInfo(const lexer::TokenInfo& val)
    {
        valueInfo_ = val;
    }

    const lexer::TokenInfo& EnumField::valueInfo() const
    {
        return valueInfo_;
    }

    void EnumField::value(const types::EnumValueType& value)
    {
        value_ = value;
    }

    const types::EnumValueType& EnumField::value() const
    {
        return value_.value();
    }

    void EnumField::incrementValue()
    {
        value_.increment();
    }

    void EnumField::accept(VisitorInterface& visitor)
    {
        visitor(*this);

        for(auto& child : children())
        {
            child->accept(visitor);
        }
    }
}}}
