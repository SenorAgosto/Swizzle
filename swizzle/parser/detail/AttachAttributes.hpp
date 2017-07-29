#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/parser/NodeStack.hpp>

namespace swizzle { namespace parser { namespace detail {

    // @attributeStack will be emptied, all attribute nodes will be appended to @node
    void attachAttributes(NodeStack& attributeStack, ast::Node::smartptr node);
}}}
