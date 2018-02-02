#include <swizzle/types/utils/IsEnum.hpp>

#include <swizzle/ast/nodes/Enum.hpp>
#include <swizzle/parser/ParserStateContext.hpp>

namespace swizzle { namespace types { namespace utils {

    bool is_enum(const parser::ParserStateContext& context, const std::string& type)
    {
        const auto iter = context.TypeCache.find(type);
        if(iter != context.TypeCache.cend())
        {
            const auto enumType = dynamic_cast<ast::nodes::Enum*>(iter->second.get());
            if(enumType)
            {
                return true;
            }
        }
    
        return false;
    }
}}}

