#include <swizzle/parser/states/StructFieldNamespaceSecondColonState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructFieldNamespaceSecondColonState::consume(const lexer::TokenInfo& token, types::NodeStack&, types::NodeStack&, types::TokenStack& tokenStack, ParserStateContext&)
    {
        const auto type = token.token().type();
        if(type == lexer::TokenType::string)
        {
            tokenStack.push(token);
            return ParserState::StructFieldNamespaceOrType;
        }

        throw SyntaxError("Expected namespace or variable name", token);
    }
}}}
