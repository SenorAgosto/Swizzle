#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/ast/AbstractTreeVertexVisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    MultilineComment::MultilineComment(const lexer::TokenInfo& info)
        : info_(info)
    {
    }

    const lexer::TokenInfo& MultilineComment::info() const
    {
        return info_;
    }
}}}
