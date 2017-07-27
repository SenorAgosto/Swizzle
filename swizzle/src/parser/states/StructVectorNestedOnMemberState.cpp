#include <swizzle/parser/states/StructVectorNestedOnMemberState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructVectorNestedOnMemberState::consume(const lexer::TokenInfo& token, NodeStack&, NodeStack&, TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::string)
        {
            return ParserState::StructVector;
        }

        throw SyntaxError("Expected member name", token);
    }
}}}
