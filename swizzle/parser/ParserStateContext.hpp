#pragma once
#include <swizzle/ast/Node.hpp>
#include <swizzle/types/EnumValue.hpp>
#include <swizzle/types/EnumValueType.hpp>

#include <cstddef>
#include <string>
#include <unordered_map>

namespace swizzle { namespace parser {

    struct ParserStateContext
    {
        std::unordered_map<std::string, ast::Node::smartptr> TypeCache; // @key is type name with namespace prefix

        std::string CurrentNamespace;
        std::size_t CurrentBitfieldBit = 0;
        types::EnumValue CurrentEnumValue = types::EnumValue(types::EnumValueType(std::uint64_t(0)));

        bool MemberIsConst = false;
    };
}}
