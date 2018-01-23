#include <swizzle/ast/nodes/DefaultValue.hpp>

#include <swizzle/ast/VisitorInterface.hpp>
#include <swizzle/types/SetValue.hpp>

namespace swizzle { namespace ast { namespace nodes {

    DefaultValue::DefaultValue(const lexer::TokenInfo& value, const std::string& underlyingType)
        : value_(value)
        , underlying_(underlyingType)
    {
    }

    const lexer::TokenInfo& DefaultValue::value() const
    {
        return value_;
    }

    const std::string& DefaultValue::underlying() const
    {
        return underlying_;
    }

    void DefaultValue::accept(VisitorInterface& visitor, Node& parent, const Node::Depth depth)
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
