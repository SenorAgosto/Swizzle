#pragma once
#include <swizzle/types/TypeCache.hpp>
#include <string>

namespace swizzle { namespace parser { namespace utils {
    
    // @typeName should be the type name with namespace
    bool NameInTypeCache(const types::TypeCacheType& typeCache, const std::string& typeName, const std::string& currentNamespace);
}}}
