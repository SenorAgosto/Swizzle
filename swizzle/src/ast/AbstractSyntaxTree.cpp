#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/ast/VisitorInterface.hpp>
#include <swizzle/ast/nodes/Root.hpp>

namespace swizzle { namespace ast {

    AbstractSyntaxTree::AbstractSyntaxTree()
        : root_(new nodes::Root())
    {
    }

    const Node::smartptr AbstractSyntaxTree::root() const
    {
        return root_;
    }

    Node::smartptr AbstractSyntaxTree::root()
    {
        return root_;
    }

    void AbstractSyntaxTree::accept(VisitorInterface& visitor)
    {
        root_->accept(visitor, ancestors_);
    }
}}
