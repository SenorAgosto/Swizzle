#pragma once 
#include <UsingIntrusivePtrIn/details/SingleThreadedReferenceCountBase.hpp>

namespace swizzle { namespace ast {
    class AbstractTreeVertexVisitorInterface;
    class AbstractTreeEdgeVisitorInterface;
    class AbstractTreeVisitorInterface;
}}

namespace swizzle { namespace ast { namespace detail {

    struct AbstractTreeTraits
    {
        using VertexVisitor = AbstractTreeVertexVisitorInterface;
        using EdgeVisitor = AbstractTreeEdgeVisitorInterface;

        using GraphVisitor = AbstractTreeVisitorInterface;
        using TreeVisitor = GraphVisitor;

        using ReferenceCountBase = UsingIntrusivePtrIn::details::SingleThreadedReferenceCountBase;
    };
}}}
