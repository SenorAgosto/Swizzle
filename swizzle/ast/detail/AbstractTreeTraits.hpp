#pragma once 
#include <UsingIntrusivePtrIn/details/SingleThreadedReferenceCountBase.hpp>

namespace swizzle { namespace ast { namespace detail {

    class AbstractTreeVertexVisitorInterface;
    class AbstractTreeEdgeVisitorInterface;
    class AbstractTreeVisitorInterface;

    struct AbstractTreeTraits
    {
        using VertexVisitor = AbstractTreeVertexVisitorInterface;
        using EdgeVisitor = AbstractTreeEdgeVisitorInterface;

        using GraphVisitor = AbstractTreeVisitorInterface;
        using TreeVisitor = GraphVisitor;

        using ReferenceCountBase = UsingIntrusivePtrIn::details::SingleThreadedReferenceCountBase;
    };
}}}
