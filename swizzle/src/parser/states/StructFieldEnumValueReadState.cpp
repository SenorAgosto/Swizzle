#include <swizzle/parser/states/StructFieldEnumValueReadState.hpp>
#include <swizzle/Exceptions.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructFieldEnumValueReadState::consume(const lexer::TokenInfo& token, types::NodeStack&, types::NodeStack&, types::TokenStack& tokenStack, ParserStateContext&)
    {
        const auto type = token.token().type();
        
        if(type == lexer::TokenType::string)
        {
            tokenStack.push(token);
            return ParserState::StructFieldEnumOrNamespaceRead;
        }
        
        throw SyntaxError("Expected token type string", token);
    }
}}}
