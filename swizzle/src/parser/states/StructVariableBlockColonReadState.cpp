#include <swizzle/parser/states/StructVariableBlockColonReadState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructVariableBlockColonReadState::consume(const lexer::TokenInfo& token, NodeStack&, TokenStack& tokenStack, ParserStateContext&)
    {
        const auto type = token.token().type();
        if(type == lexer::TokenType::string)
        {
            tokenStack.push(token);
            return ParserState::StructVariableBlockOnField;
        }

        throw SyntaxError("Expected field name of type string", token);
    }
}}}
