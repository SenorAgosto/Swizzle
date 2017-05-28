#include <swizzle/ast/nodes/StringLiteral.hpp>
#include <swizzle/ast/AbstractTreeVertexVisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    StringLiteral::StringLiteral(const lexer::TokenInfo& info)
        : info_(info)
    {
    }

    const lexer::TokenInfo& StringLiteral::info() const
    {
        return info_;
    }
}}}
