#pragma once

#include <ObjectGraph/DirectedGraph.hpp>

#include <swizzle/ast/detail/AbstractTreeTraits.hpp>

#include <swizzle/ast/AbstractTreeEdgeVisitorInterface.hpp>
#include <swizzle/ast/AbstractTreeVertexVisitorInterface.hpp>
#include <swizzle/ast/AbstractTreeVisitorInterface.hpp>

#include <swizzle/ast/Node.hpp>

namespace swizzle { namespace ast {

    class AbstractSyntaxTree : public ObjectGraph::DirectedGraph<detail::AbstractTreeTraits>
    {
    public:
        AbstractSyntaxTree();

        Node::smartptr root();

    private:
        Node::smartptr root_;
    };
}}
