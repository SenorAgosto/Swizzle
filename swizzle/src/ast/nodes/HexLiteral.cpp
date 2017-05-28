#include <swizzle/ast/nodes/HexLiteral.hpp>
#include <swizzle/ast/AbstractTreeVertexVisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    HexLiteral::HexLiteral(const lexer::TokenInfo& info)
        : info_(info)
    {
    }

    const lexer::TokenInfo& HexLiteral::info() const
    {
        return info_;
    }
}}}
