#pragma once 
#include <swizzle/ast/Vertex.hpp>
#include <deque>

namespace swizzle { namespace ast {

    class Node : public Vertex
    {
    public:
        const std::deque<Node::smartptr>& children() const;
        void append(Node::smartptr node);

    private:
        std::deque<Node::smartptr> children_;
    };
}}
