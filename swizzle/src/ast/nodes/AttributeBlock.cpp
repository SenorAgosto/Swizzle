#include <swizzle/ast/nodes/AttributeBlock.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    AttributeBlock::AttributeBlock(const lexer::TokenInfo& info)
        : info_(info)
    {
    }

    const lexer::TokenInfo& AttributeBlock::info() const
    {
        return info_;
    }

    void AttributeBlock::accept(VisitorInterface& visitor)
    {
        visitor(*this);

        for(auto& child : children())
        {
            child->accept(visitor);
        }
    }

}}}
