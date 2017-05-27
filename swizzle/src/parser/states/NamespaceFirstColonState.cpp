#include <swizzle/parser/states/NamespaceFirstColonState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState NamespaceFirstColonState::consume(const lexer::TokenInfo& token, NodeStack&, TokenStack&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::colon)
        {
            return ParserState::StartNamespace;
        }

        throw SyntaxError("Expected : found '" + token.token().to_string() + "'", token);
    }
}}}
