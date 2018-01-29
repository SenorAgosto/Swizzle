#pragma once
#include <swizzle/ast/Node.hpp>
#include <swizzle/types/NodeStack.hpp>

namespace swizzle { namespace types { namespace utils {

    template<class Node, typename... Args>
    ast::Node::smartptr appendNode(NodeStack& nodeStack, Args&&... args)
    {
        ast::Node::smartptr node = new Node(std::forward<Args>(args)...);

        auto& top = dynamic_cast<ast::Node&>(*nodeStack.top());
        top.append(node);

        return node;
    }
}}}
