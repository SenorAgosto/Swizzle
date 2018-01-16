#pragma once
#include <swizzle/ast/Node.hpp>
#include <unordered_map>

namespace swizzle { namespace types {

    // TypeCacheType is keyed by type name with namespace prefix
    using TypeCacheType = std::unordered_map<std::string, ast::Node::smartptr>;
}}
