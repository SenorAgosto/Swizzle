#pragma once

#include <ObjectGraph/Vertex.hpp>
#include <swizzle/ast/detail/AbstractTreeTraits.hpp>

#include <deque>

namespace swizzle { namespace ast {

    using Vertex = ObjectGraph::VertexImpl<detail::AbstractTreeTraits>;
}}
