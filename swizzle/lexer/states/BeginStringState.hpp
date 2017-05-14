#pragma once 
#include <swizzle/lexer/TokenizerStateInterface.hpp>

#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/ResetToken.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/lexer/TokenProducer.hpp>
#include <swizzle/lexer/TokenizerState.hpp>

#include <cctype>

namespace swizzle { namespace lexer { namespace states {

    template<class CreateTokenCallback>
    class BeginStringState
        : public TokenizerStateInterface
        , private TokenProducer<CreateTokenCallback>
    {
    public:
        BeginStringState(CreateTokenCallback callback)
            : TokenProducer<CreateTokenCallback>(callback)
        {
        }

        TokenizerState consume(const boost::string_view& source, const std::size_t position, FileInfo& fileInfo, Token& token) override
        {
            const char c = source.at(position);

            if(c == '/')
            {
                fileInfo = this->produceToken(token, fileInfo);
                token = ResetToken(source, position, TokenType::comment);

                return TokenizerState::FirstSlash;
            }

            if(c == '"')
            {
                fileInfo = this->produceToken(token, fileInfo);
                token = ResetToken(source, position, TokenType::string_literal);

                return TokenizerState::StringLiteral;
            }

            if(c == '\'')
            {
                fileInfo = this->produceToken(token, fileInfo);
                token = ResetToken(source, position, TokenType::char_literal);

                return TokenizerState::CharLiteral;
            }

            static const std::string tokenProducers("@=[]{}.;:");
            if(tokenProducers.find_first_of(c) != std::string::npos)
            {
                fileInfo = this->produceToken(token, fileInfo);

                token = ResetToken(source, position, CharToTokenType(c));
                token.expand(source);

                fileInfo = this->produceToken(token, fileInfo);
                token = ResetToken(source, position + 1, TokenType::string);

                return TokenizerState::Init;
            }

            static const std::string whitespace(" \t\r\n");
            if(whitespace.find_first_of(c) != std::string::npos)
            {
                fileInfo = this->produceToken(token, fileInfo);
                token = ResetToken(source, position + 1, TokenType::string);

                return TokenizerState::Init;
            }

            token.expand(source);
            fileInfo.advanceBy(c);

            return TokenizerState::BeginString;
        }
    };
}}}
