#pragma once 
#include <swizzle/parser/NodeStack.hpp>

namespace swizzle { namespace parser { namespace detail {

    template<class NodeType>
    bool nodeStackTopIs(const NodeStack& nodeStack)
    {
        return dynamic_cast<NodeType*>(nodeStack.top().get()) != nullptr;
    }
}}}
