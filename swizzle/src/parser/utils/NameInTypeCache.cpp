#include <swizzle/parser/utils/NameInTypeCache.hpp>

namespace swizzle { namespace parser { namespace utils {
    
    // @typeName should be the type name with namespace
    bool NameInTypeCache(const types::TypeCacheType& typeCache, const std::string& typeName, const std::string& currentNamespace)
    {
        // first prepend current namespace and try the lookup,
        const std::string type = currentNamespace + "::" + typeName;
        const auto iter1 = typeCache.find(type);
        
        if(iter1 == typeCache.cend())
        {
            // try again without current namespace
            const auto iter2 = typeCache.find(typeName);
            return iter2 != typeCache.cend();
        }
        
        return true;
    }
}}}

