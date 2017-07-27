#include <swizzle/parser/states/StructFieldNameState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructFieldNameState::consume(const lexer::TokenInfo& token, NodeStack&, NodeStack&, TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::end_statement)
        {
            return ParserState::StructStartScope;
        }

        throw SyntaxError("Expected ';'", token);
    }
}}}
