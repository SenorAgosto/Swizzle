#include <swizzle/parser/states/UsingReadEqualState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

#include <sstream>

namespace swizzle { namespace parser { namespace states {

    ParserState UsingReadEqualState::consume(const lexer::TokenInfo& token, NodeStack&, TokenStack& tokenStack, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if((type == lexer::TokenType::type) || (type == lexer::TokenType::string))
        {
            tokenStack.push(token);
            return ParserState::UsingTypeRead;
        }

        std::stringstream ss;
        ss << type;

        throw SyntaxError("Expected type or type name, found " + ss.str() + " (" + token.token().to_string() + ")", token);
    }
}}}
