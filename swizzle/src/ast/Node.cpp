#include <swizzle/ast/Node.hpp>

namespace swizzle { namespace ast {

    const std::deque<Node::smartptr>& Node::children() const
    {
        return children_;
    }

    void Node::append(Node::smartptr node)
    {
        children_.push_back(node);
    }

    bool Node::empty() const
    {
        return children_.empty();
    }
}}
