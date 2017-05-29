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

        Node::smartptr root();
        void accept(VisitorInterface& visitor);

    private:
        Node::smartptr root_;
    };
}}
