#include <swizzle/ast/nodes/EnumField.hpp>

#include <swizzle/ast/AncestorInfo.hpp>

#include <swizzle/ast/AncestorInfo.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    EnumField::EnumField(const lexer::TokenInfo& name, const lexer::TokenInfo& underlyingType)
        : name_(name)
        , underlying_(underlyingType)
        , value_(0)
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

    void EnumField::value(const std::uint64_t value)
    {
        value_ = value;
    }

    std::uint64_t EnumField::value() const
    {
        return value_;
    }

    void EnumField::accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Depth depth)
    {
        visitor(ancestors, *this);
        
        if(depth == Depth::One) return;
        ancestors.push(*this);
        
        for(auto& child : children())
        {
            child->accept(visitor, ancestors, depth);
        }
        
        ancestors.pop();
    }
}}}
