#pragma once 
#include <UsingIntrusivePtrIn/details/SingleThreadedReferenceCountBase.hpp>

namespace swizzle { namespace ast { namespace detail {

    class AbstractTreeVertexVisitorInterface;
    class AbstractTreeEdgeVisitorInterface;
    class AbstractTreeVisitorInterface;

    struct SwizzleAbstractTreeTraits
    {
        using VertexVisitor = AbstractTreeVertexVisitorInterface;
        using EdgeVisitor = AbstractTreeEdgeVisitorInterface;
        using TreeVisitor = AbstractTreeVisitorInterface;

        using ReferenceCountBase = UsingIntrusivePtrIn::details::SingleThreadedReferenceCountBase;
    };
}}}
