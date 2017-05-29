#include <swizzle/ast/nodes/EnumField.hpp>
#include <swizzle/ast/AbstractTreeVertexVisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    EnumField::EnumField(const lexer::TokenInfo& name, const lexer::TokenInfo& underlyingType)
        : name_(name)
        , underlying_(underlyingType)
        , value_(0)
        , isNegative_(false)
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

    bool EnumField::isNegative() const
    {
        return isNegative_;
    }

    std::size_t EnumField::value() const
    {
        return value_;
    }
}}}
