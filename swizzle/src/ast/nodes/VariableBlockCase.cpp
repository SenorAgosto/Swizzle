#include <swizzle/ast/nodes/VariableBlockCase.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    void VariableBlockCase::value(const lexer::TokenInfo& value)
    {
        value_ = value;
    }

    const lexer::TokenInfo& VariableBlockCase::value() const
    {
        return value_;
    }

    void VariableBlockCase::type(const lexer::TokenInfo& type)
    {
        type_ = type;
    }

    lexer::TokenInfo VariableBlockCase::type() const
    {
        return type_;
    }
    
    void VariableBlockCase::accept(VisitorInterface& visitor, Node& parent, const Node::Depth depth)
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
