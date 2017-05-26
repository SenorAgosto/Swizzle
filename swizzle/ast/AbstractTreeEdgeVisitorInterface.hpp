#pragma once 
#include <swizzle/ast/Edge.hpp>

namespace swizzle { namespace ast {

    class AbstractTreeEdgeVisitorInterface
    {
    public:
        virtual ~AbstractTreeEdgeVisitorInterface(){}

        virtual void operator()(Edge& edge) = 0;
    };
}}
