#include <swizzle/parser/states/StructFieldValueReadState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/lexer/TokenType.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructFieldValueReadState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, NodeStack&, TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::end_statement)
        {
            nodeStack.pop();
            return ParserState::StructStartScope;
        }

        throw SyntaxError("Expected ';'", token);
    }
}}}
