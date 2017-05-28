#include <swizzle/ast/nodes/NumericLiteral.hpp>
#include <swizzle/ast/AbstractTreeVertexVisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    NumericLiteral::NumericLiteral(const lexer::TokenInfo& info)
        : info_(info)
    {
    }

    const lexer::TokenInfo& NumericLiteral::info() const
    {
        return info_;
    }
}}}
