#include <swizzle/ast/nodes/DefaultStringValue.hpp>

#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    DefaultStringValue::DefaultStringValue(const lexer::TokenInfo& value, const std::string& underlyingType, const std::ptrdiff_t length)
        : value_(value)
        , underlying_(underlyingType)
        , length_(length)
    {
    }

    const lexer::TokenInfo& DefaultStringValue::value() const
    {
        return value_;
    }

    const std::string& DefaultStringValue::underlying() const
    {
        return underlying_;
    }

    std::ptrdiff_t DefaultStringValue::length() const
    {
        return length_;
    }

    void DefaultStringValue::accept(VisitorInterface& visitor, Node& parent, const Node::Depth depth)
    {
        visitor(parent, *this);
        if(depth == Depth::One) return;
        
        for(auto& child : children())
        {
            auto parent = this;
            child->accept(visitor, *parent, depth);
        }
    }
}}}
