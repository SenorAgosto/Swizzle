#pragma once 
#include <cstdint>
#include <stdexcept>
#include <string>

namespace swizzle { namespace lexer {
    class TokenInfo;
    enum class TokenizerState : std::uint8_t;
}}

namespace swizzle { namespace parser {
    enum class ParserState : std::uint8_t;
}}

namespace swizzle {

    class ParserError : public std::runtime_error
    {
    public:
        ParserError(const std::string& reason);
    };
    
    class TokenizerError : public std::runtime_error
    {
    public:
        TokenizerError(const std::string& reason);
    };
    
    class UnknownTokenizerState : public TokenizerError
    {
    public:
        UnknownTokenizerState(const lexer::TokenizerState state);
    };
    
    class SyntaxError : public ParserError
    {
    public:
        SyntaxError(const std::string& error, const lexer::TokenInfo& token);
    };
    
    class UnknownParserState : public ParserError
    {
    public:
        UnknownParserState(const parser::ParserState state);
    };

}
