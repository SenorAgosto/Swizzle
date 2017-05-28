#include <swizzle/ast/nodes/CharLiteral.hpp>
#include <swizzle/ast/AbstractTreeVertexVisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    CharLiteral::CharLiteral(const lexer::TokenInfo& info)
        : info_(info)
    {
    }

    const lexer::TokenInfo& CharLiteral::info() const
    {
        return info_;
    }
}}}
