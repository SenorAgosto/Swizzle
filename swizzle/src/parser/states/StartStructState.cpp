#include <swizzle/parser/states/StartStructState.hpp>

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StartStructState::consume(const lexer::TokenInfo& token, ast::AbstractSyntaxTree& ast, NodeStack& nodeStack, TokenStack& tokenStack)
    {
        // TODO: implement
        return ParserState::Init;
    }
}}}
