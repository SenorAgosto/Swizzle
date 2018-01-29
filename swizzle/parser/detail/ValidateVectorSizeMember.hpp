#pragma once 
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace lexer {
    class TokenInfo;
}}

namespace swizzle { namespace parser {
    struct ParserStateContext;
}}

namespace swizzle { namespace parser { namespace detail {

    // iterate through the token stack, ensure the members making
    // up the vector size argument all exist and are of the correct
    // types.
    void validateVectorSizeMember(const lexer::TokenInfo& token, const types::NodeStack& nodeStack, const types::TokenStack& tokenStack, const ParserStateContext& context);
}}}
