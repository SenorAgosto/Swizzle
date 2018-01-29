#include <swizzle/parser/states/UsingReadEqualState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState UsingReadEqualState::consume(const lexer::TokenInfo& token, types::NodeStack&, types::NodeStack&, types::TokenStack& tokenStack, ParserStateContext&)
    {
        const auto type = token.token().type();

        if((type == lexer::TokenType::type) || (type == lexer::TokenType::string))
        {
            tokenStack.push(token);
            return ParserState::UsingTypeRead;
        }

        throw SyntaxError("Expected type or type name", token);
    }
}}}
