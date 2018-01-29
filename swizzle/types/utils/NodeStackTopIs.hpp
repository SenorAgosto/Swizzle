#pragma once 
#include <swizzle/types/NodeStack.hpp>

namespace swizzle { namespace types { namespace utils {

    template<class NodeType>
    bool nodeStackTopIs(const NodeStack& nodeStack)
    {
        return dynamic_cast<NodeType*>(nodeStack.top().get()) != nullptr;
    }
}}}
