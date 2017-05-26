#pragma once 
#include <swizzle/ast/Vertex.hpp>

namespace swizzle { namespace ast {
    class Node;
}}

namespace swizzle { namespace ast { namespace vertices {

}}}

namespace swizzle { namespace ast {

    class AbstractTreeVertexVisitorInterface
    {
    public:
        virtual ~AbstractTreeVertexVisitorInterface(){}

        virtual void operator()(Vertex& vertex) = 0;
        virtual void operator()(Node& node) = 0;
    };
}}
