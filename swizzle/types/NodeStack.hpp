#pragma once 
#include <swizzle/ast/Node.hpp>
#include <stack>

namespace swizzle { namespace types {

    using NodeStack = std::stack<ast::Node::smartptr>;
}}
