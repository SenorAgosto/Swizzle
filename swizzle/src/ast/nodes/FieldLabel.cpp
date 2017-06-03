#include <swizzle/ast/nodes/FieldLabel.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    FieldLabel::FieldLabel(const lexer::TokenInfo& info)
        : info_(info)
    {
    }

    const lexer::TokenInfo& FieldLabel::info() const
    {
        return info_;
    }

    void FieldLabel::colon(const lexer::TokenInfo& colon)
    {
        colon_ = colon;
    }

    const lexer::TokenInfo& FieldLabel::colon() const
    {
        return colon_;
    }

    void FieldLabel::accept(VisitorInterface& visitor)
    {
        visitor(*this);

        for(auto& child : children())
        {
            child->accept(visitor);
        }
    }
}}}
