#pragma once 

namespace swizzle { namespace ast { namespace edges {
}}}

namespace swizzle { namespace ast {

    class AbstractTreeEdgeVisitorInterface
    {
    public:
        virtual ~AbstractTreeEdgeVisitorInterface(){}

        virtual void operator()(Edge& edge) = 0;
    };
}}
