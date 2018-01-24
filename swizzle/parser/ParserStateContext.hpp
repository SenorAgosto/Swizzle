#pragma once
#include <swizzle/ast/Node.hpp>
#include <swizzle/types/EnumValue.hpp>
#include <swizzle/types/EnumValueType.hpp>
#include <swizzle/types/TypeCache.hpp>

#include <cstddef>
#include <string>
#include <limits>
#include <unordered_set>

namespace swizzle { namespace lexer {
    class TokenInfo;
}}

namespace swizzle { namespace parser {

    struct ParserStateContext
    {
        types::TypeCacheType TypeCache;
        ast::Node::smartptr CurrentVariableOnFieldType = nullptr;       // the pointer to the field we're variable on, so we can query the type

        std::string CurrentNamespace;
        std::intmax_t CurrentBitfieldBit = std::numeric_limits<std::intmax_t>::lowest();

        types::EnumValue CurrentEnumValue = types::EnumValue(types::EnumValueType(std::uint64_t(0)));
        void AllocateEnumValue(const lexer::TokenInfo& token, const types::EnumValue& value);  // mark value as taken, reusing values is a syntax error.
        void ClearEnumValueAllocations();

        bool MemberIsConst = false;

        void AllocateFieldLabel(const lexer::TokenInfo& token);
        void ClearFieldLabels();
        
    private:
        std::unordered_set<std::intmax_t> UsedEnumValues;
        std::unordered_set<std::string> UsedFieldLabels;
    };
}}
