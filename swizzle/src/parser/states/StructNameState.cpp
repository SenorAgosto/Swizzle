#include <swizzle/parser/states/StructNameState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructNameState::consume(const lexer::TokenInfo& token, types::NodeStack&, types::NodeStack&, types::TokenStack&, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::l_brace)
        {
            context.MemberIsConst = false;
            return ParserState::StructStartScope;
        }
        
        if(type == lexer::TokenType::colon)
        {
            return ParserState::StructBaseColonRead;
        }

        throw SyntaxError("Expected '{'", token);
    }
}}}
