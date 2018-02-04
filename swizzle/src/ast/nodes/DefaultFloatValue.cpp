#include <swizzle/ast/nodes/DefaultFloatValue.hpp>

#include <swizzle/ast/AncestorInfo.hpp>
#include <swizzle/ast/VisitorInterface.hpp>
#include <swizzle/types/SetValue.hpp>

namespace swizzle { namespace ast { namespace nodes {

    DefaultFloatValue::DefaultFloatValue(const lexer::TokenInfo& value, const std::string& underlyingType)
        : value_(value)
        , underlying_(underlyingType)
    {
    }

    const lexer::TokenInfo& DefaultFloatValue::value() const
    {
        return value_;
    }

    const std::string& DefaultFloatValue::underlying() const
    {
        return underlying_;
    }

    void DefaultFloatValue::accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Node::Depth depth)
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
