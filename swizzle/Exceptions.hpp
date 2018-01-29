#pragma once
#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

#include <boost/utility/string_view.hpp> 
#include <cstdint>
#include <stdexcept>
#include <string>

namespace swizzle { namespace lexer {
    enum class TokenizerState : std::uint8_t;
    enum class TokenType : std::uint8_t;
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

    class AliasedTypeIsNotDefinedException : public ParserError
    {
    public:
        AliasedTypeIsNotDefinedException(const std::string& existingType, const lexer::FileInfo& fileInfo);
    };

    class RedefinitionOfAliasNameException : public ParserError
    {
    public:
        RedefinitionOfAliasNameException(const std::string& aliasName, const lexer::FileInfo& fileInfo);
    };

    class RedefinitionOfStructTypeException : public ParserError
    {
    public:
        RedefinitionOfStructTypeException(const std::string& structName, const lexer::FileInfo& fileInfo);
    };

    class StreamEmpty : std::runtime_error
    {
    public:
        StreamEmpty();
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

    class TokenizerSyntaxError : public TokenizerError
    {
    public:
        TokenizerSyntaxError(const boost::string_view& source, const std::size_t position, const lexer::FileInfo& info, const std::string& reason);
        
        boost::string_view source() const { return source_; }
        std::size_t position() const { return position_; }
        const lexer::FileInfo& file_info() const { return info_; }
        
    private:
        boost::string_view source_;
        std::size_t position_;
        
        lexer::FileInfo info_;
    };

    class SyntaxError : public ParserError
    {
    public:
        SyntaxError(const std::string& error, const lexer::TokenInfo& token);
        SyntaxError(const std::string& error, const std::string& found, const lexer::TokenInfo& token);
        
        const lexer::TokenInfo& token() const { return token_; }
        
    private:
        lexer::TokenInfo token_;
    };
    
    class SyntaxErrorWithoutToken : public ParserError
    {
    public:
        SyntaxErrorWithoutToken(const std::string& error, const std::string& found, const lexer::FileInfo& info);
        const lexer::FileInfo& file_info() const { return info_; }
        
    private:
        lexer::FileInfo info_;
    };

    class InvalidStreamInput : public SyntaxError
    {
    public:
        InvalidStreamInput(const lexer::TokenInfo& token, const std::string& sv);
    };

    class StreamInputCausesOverflow : public SyntaxError
    {
    public:
        StreamInputCausesOverflow(const lexer::TokenInfo& token, const std::string& s);
    };

    class StreamInputCausesUnderflow : public SyntaxError
    {
    public:
        StreamInputCausesUnderflow(const lexer::TokenInfo& token, const std::string& s);
    };

    class UnknownParserState : public ParserError
    {
    public:
        UnknownParserState(const parser::ParserState state);
    };

}
