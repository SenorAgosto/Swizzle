#pragma once
#include <swizzle/ast/Node.hpp>
#include <swizzle/types/EnumValue.hpp>
#include <swizzle/types/EnumValueType.hpp>

#include <cstddef>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace swizzle { namespace lexer {
    class TokenInfo;
}}

namespace swizzle { namespace parser {

    struct ParserStateContext
    {
        std::unordered_map<std::string, ast::Node::smartptr> TypeCache; // @key is type name with namespace prefix
        ast::Node::smartptr CurrentVariableOnFieldType = nullptr;       // the pointer to the field we're variable on, so we can query the type

        std::string CurrentNamespace;
        std::size_t CurrentBitfieldBit = 0;

        types::EnumValue CurrentEnumValue = types::EnumValue(types::EnumValueType(std::uint64_t(0)));
        void AllocateEnumValue(const lexer::TokenInfo& token, const types::EnumValue& value);  // mark value as taken, reusing values is a syntax error.
        void ClearEnumValueAllocations();

        bool MemberIsConst = false;

    private:
        std::unordered_set<std::intmax_t> UsedEnumValues;
    };
}}
