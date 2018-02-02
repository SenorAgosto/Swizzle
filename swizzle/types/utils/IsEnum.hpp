#pragma once
#include <string>

namespace swizzle { namespace parser {
    struct ParserStateContext;
}}

namespace swizzle { namespace types { namespace utils {

    bool is_enum(const parser::ParserStateContext& context, const std::string& type);
}}}
