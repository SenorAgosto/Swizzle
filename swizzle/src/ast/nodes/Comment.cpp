#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/AbstractTreeVertexVisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    Comment::Comment(const lexer::TokenInfo& info)
        : info_(info)
    {
    }

    const lexer::TokenInfo& Comment::info() const
    {
        return info_;
    }
}}}
