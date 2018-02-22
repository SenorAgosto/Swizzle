#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

namespace swizzle { namespace ast { namespace nodes {

    class Comment : public Node
    {
    public:
        Comment(const lexer::TokenInfo& info);

        const lexer::TokenInfo& info() const;

        void accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Depth depth = Depth::All) override;

    private:
        const lexer::TokenInfo info_;
    };
}}}
