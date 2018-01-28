#include <swizzle/parser/states/StartNamespaceState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StartNamespaceState::consume(const lexer::TokenInfo& token, types::NodeStack&, types::NodeStack&, types::TokenStack& tokenStack, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::string)
        {
            tokenStack.push(token);
            return ParserState::NamespaceValue;
        }

        throw SyntaxError("Expecting namespace name", token);
    }
}}}
