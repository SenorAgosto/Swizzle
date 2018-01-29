#pragma once 
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser {
    struct ParserStateContext;
}}

namespace swizzle { namespace types { namespace utils {

    lexer::TokenInfo createMember(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::TokenStack& tokenStack, const parser::ParserStateContext& context, const std::string& onEmptyTokenStack);

}}}
