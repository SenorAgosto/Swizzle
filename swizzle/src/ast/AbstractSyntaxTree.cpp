#include <swizzle/ast/AbstractSyntaxTree.hpp>

namespace swizzle { namespace ast {

    AbstractSyntaxTree::AbstractSyntaxTree()
        : root_(new Node())
    {
    }

    Node::smartptr AbstractSyntaxTree::root()
    {
        return root_;
    }
}}
