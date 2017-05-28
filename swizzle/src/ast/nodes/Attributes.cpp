#include <swizzle/ast/nodes/Attribute.hpp>
#include <swizzle/ast/AbstractTreeVertexVisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    Attribute::Attribute(const lexer::TokenInfo& info)
        : info_(info)
    {
    }

    const lexer::TokenInfo& Attribute::info() const
    {
        return info_;
    }
}}}
