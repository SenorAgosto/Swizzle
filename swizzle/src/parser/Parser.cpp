#include <swizzle/parser/Parser.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

namespace swizzle { namespace parser {

    Parser::Parser()
        : state_(ParserState::Init)
    {
        nodeStack_.push(ast_.root());
    }

    void Parser::consume(const lexer::TokenInfo& token)
    {
        state_ = states_.consume(state_, token, nodeStack_, tokenStack_, context_);
    }

    const ast::AbstractSyntaxTree& Parser::ast() const
    {
        return ast_;
    }
}}
