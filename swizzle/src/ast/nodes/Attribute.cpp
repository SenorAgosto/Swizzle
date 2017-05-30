#include <swizzle/ast/nodes/Attribute.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    Attribute::Attribute(const lexer::TokenInfo& info)
        : info_(info)
    {
    }

    const lexer::TokenInfo& Attribute::info() const
    {
        return info_;
    }

    void Attribute::accept(VisitorInterface& visitor)
    {
        visitor(*this);

        for(auto& child : children())
        {
            child->accept(visitor);
        }
    }

}}}
