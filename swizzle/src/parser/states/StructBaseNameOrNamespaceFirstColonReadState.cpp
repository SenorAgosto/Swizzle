#include <swizzle/parser/states/StructBaseNameOrNamespaceFirstColonReadState.hpp>
#include <swizzle/Exceptions.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructBaseNameOrNamespaceFirstColonReadState::consume(const lexer::TokenInfo& token, types::NodeStack&, types::NodeStack&, types::TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();
        
        if(type == lexer::TokenType::colon)
        {
            return ParserState::StructBaseColonRead;
        }
        
        throw SyntaxError("Expected ':'", token);
    }
}}}
