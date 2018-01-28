#include <swizzle/parser/states/EnumFieldValueReadState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState EnumFieldValueReadState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack&, types::TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::comma)
        {
            nodeStack.pop();
            return ParserState::EnumStartScope;
        }

        throw SyntaxError("Expected ','", token);
    }
}}}
