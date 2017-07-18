#pragma once
#include <swizzle/ast/Node.hpp>

namespace swizzle { namespace ast {
    class VisitorInterface;
}}

namespace swizzle { namespace ast {

    class AbstractSyntaxTree
    {
    public:
        AbstractSyntaxTree();

        const Node::smartptr root() const;
        Node::smartptr root();

        void accept(VisitorInterface& visitor);

    private:
        Node::smartptr root_;
    };
}}
