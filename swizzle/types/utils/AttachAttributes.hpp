#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/types/NodeStack.hpp>

namespace swizzle { namespace types { namespace utils {

    // @attributeStack will be emptied, all attribute nodes will be appended to @node
    void attachAttributes(NodeStack& attributeStack, ast::Node::smartptr node);
}}}
