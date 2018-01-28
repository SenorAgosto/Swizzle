#include <swizzle/parser/states/StructFieldNamespaceFirstColonState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructFieldNamespaceFirstColonState::consume(const lexer::TokenInfo& token, types::NodeStack&, types::NodeStack&, types::TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::colon)
        {
            return ParserState::StructFieldNamespaceSecondColon;
        }

        throw SyntaxError("Expected ':'", token);
    }
}}}
