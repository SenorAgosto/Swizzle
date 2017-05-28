#pragma once
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/ast/Node.hpp>

namespace swizzle { namespace parser { namespace detail {

    template<class Node, typename... Args>
    void appendNode(NodeStack& nodeStack, Args&&... args)
    {
        ast::Node::smartptr node = new Node(std::forward<Args>(args)...);

        auto& top = dynamic_cast<ast::Node&>(*nodeStack.top());
        top.append(node);
    }
}}}
