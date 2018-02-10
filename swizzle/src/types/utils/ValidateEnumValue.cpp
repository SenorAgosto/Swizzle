#include <swizzle/types/utils/ValidateEnumValue.hpp>

#include <swizzle/ast/nodes/Enum.hpp>
#include <swizzle/ast/nodes/EnumField.hpp>
#include <swizzle/ast/Matcher.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <iterator>
#include <string>
#include <vector>

namespace swizzle { namespace types { namespace utils {

    bool validateEnumValue(const parser::ParserStateContext& context, const std::string& enumType, const lexer::TokenInfo& token)
    {
        std::vector<std::string> parts;
        const auto enumValue = token.token().value().to_string();
        
        boost::algorithm::split(parts, enumValue, boost::algorithm::is_any_of(":"));
        
        parts.erase(std::remove_if(begin(parts), end(parts), [](const auto part) -> bool {
            return part.empty();
        }), end(parts));
        
        if(parts.size() < 2)
        {
            return false;
        }
        
        const std::string& enum_value = parts.back();
        std::string enum_name;
        
        for(auto iter = parts.cbegin(), end = std::prev(parts.cend()); iter != end; ++iter)
        {
            enum_name += *iter;
        }
        
        auto info = context.SymbolTable.find(context.CurrentNamespace, enum_name, SyntaxError("Enum symbol not found: '" + enum_name + "'", token));
        
        const auto enum_node = info.node();
        const auto& Enum = static_cast<ast::nodes::Enum&>(*enum_node);
            
        if(Enum.name() != enumType)
        {
            throw SyntaxError("Mismatch enum types in assignment of default value", token);
        }
            
        auto matcher = ast::Matcher().getChildrenOf<ast::nodes::EnumField>().bind("field");
        
        if(matcher(enum_node))
        {
            const auto matches = matcher.all_bound("field");
            for(const auto match : matches)
            {
                const auto& field = static_cast<ast::nodes::EnumField&>(*match);
                if(field.name().token().value() == enum_value)
                {
                    return true;
                }
            }
        }

        throw SyntaxError("Enum value not declared on enum type", token);
    }
}}}
