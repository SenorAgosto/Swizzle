#pragma once 
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace lexer {
    class TokenInfo;
}}

namespace swizzle { namespace parser {
    struct ParserStateContext;
}}

namespace swizzle { namespace parser { namespace detail {

    // iterate through the token stack, ensure the members making
    // up the variable block size argument all exist and are of the correct
    // types.
    // @returns a pointer to the ast::nodes::StructField so the type can be queried
    ast::Node::smartptr validateVariableBlockSizeMember(const lexer::TokenInfo& token, const NodeStack& nodeStack, const TokenStack& tokenStack, const ParserStateContext& context);
}}}
