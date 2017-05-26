#pragma once 
#include <swizzle/ast/Node.hpp>
#include <stack>

namespace swizzle { namespace parser {

    using NodeStack = std::stack<ast::Node::smartptr>;
}}
