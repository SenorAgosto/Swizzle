#include <swizzle/parser/states/StructVariableBlockOnNestedFieldState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructVariableBlockOnNestedFieldState::consume(const lexer::TokenInfo& token, types::NodeStack&, types::NodeStack&, types::TokenStack& tokenStack, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::string)
        {
            tokenStack.push(token);
            return ParserState::StructVariableBlockOnField;
        }

        throw SyntaxError("Expected member name (string token)", token);
    }
}}}
