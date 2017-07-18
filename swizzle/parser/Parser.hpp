#pragma once 

#include <swizzle/ast/AbstractSyntaxTree.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserState.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/ParserStatesPack.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace lexer {
    class TokenInfo;
}}

namespace swizzle { namespace parser {

    class Parser
    {
    public:
        Parser();

        // parse token
        void consume(const lexer::TokenInfo& token);

        // return the AST
        const ast::AbstractSyntaxTree& ast() const;

    private:
        ParserStatesPack states_;
        ParserState state_;

        NodeStack nodeStack_;
        TokenStack tokenStack_;
        ParserStateContext context_;

        ast::AbstractSyntaxTree ast_;
    };
}}
