#pragma once

#include <swizzle/ast/detail/AbstractTreeTraits.hpp>
#include <ObjectGraph/Edge.hpp>

namespace swizzle { namespace ast {

    using Edge = ObjectGraph::EdgeImpl<detail::AbstractTreeTraits>;
}}
