#pragma once 
#include <swizzle/ast/Node.hpp>

namespace swizzle { namespace ast { namespace vertices {

}}}

namespace swizzle { namespace ast {

    class AbstractTreeVertexVisitorInterface
    {
    public:
        virtual ~AbstractTreeVertexVisitorInterface(){}

        virtual void operator()(Node& node) = 0;
    };
}}
