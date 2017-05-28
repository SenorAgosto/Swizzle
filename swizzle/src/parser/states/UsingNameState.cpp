#include <swizzle/parser/states/UsingNameState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/TokenStack.hpp>

#include <sstream>

namespace swizzle { namespace parser { namespace states {

    ParserState UsingNameState::consume(const lexer::TokenInfo& token, NodeStack&, TokenStack&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::equal)
        {
            return ParserState::UsingReadEqual;
        }

        std::stringstream ss;
        ss << type;

        throw SyntaxError("Expected '=' found " + ss.str() + " (" + token.token().to_string() + ")", token);
    }
}}}
