#include <swizzle/ast/nodes/FieldLabel.hpp>

#include <swizzle/ast/AncestorInfo.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    FieldLabel::FieldLabel(const lexer::TokenInfo& label)
        : label_(label)
    {
    }

    const lexer::TokenInfo& FieldLabel::label() const
    {
        return label_;
    }

    void FieldLabel::colon(const lexer::TokenInfo& colon)
    {
        colon_ = colon;
    }

    const lexer::TokenInfo& FieldLabel::colon() const
    {
        return colon_;
    }

    void FieldLabel::accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Node::Depth depth)
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
