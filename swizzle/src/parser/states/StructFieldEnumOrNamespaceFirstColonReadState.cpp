#include <swizzle/parser/states/StructFieldEnumOrNamespaceFirstColonReadState.hpp>
#include <swizzle/Exceptions.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructFieldEnumOrNamespaceFirstColonReadState::consume(const lexer::TokenInfo& token, types::NodeStack&, types::NodeStack&, types::TokenStack& tokenStack, ParserStateContext&)
    {
        const auto type = token.token().type();
        
        if(type == lexer::TokenType::colon)
        {
            tokenStack.push(token);
            return ParserState::StructFieldEnumValueRead;
        }
        
        throw SyntaxError("Expected ':'", token);
    }
}}}


