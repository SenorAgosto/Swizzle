#pragma once 
#include <swizzle/ast/Node.hpp>
#include <deque>

namespace swizzle { namespace parser {

    using NodeStack = std::deque<ast::Node::smartptr>;
}}
