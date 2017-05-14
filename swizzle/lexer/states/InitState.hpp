#pragma once 
#include <swizzle/lexer/TokenizerStateInterface.hpp>

#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/ResetToken.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/lexer/TokenProducer.hpp>
#include <swizzle/lexer/TokenizerState.hpp>
#include <swizzle/lexer/TokenType.hpp>

#include <cctype>

namespace swizzle { namespace lexer { namespace states {

    template<class CreateTokenCallback>
    class InitState
        : public TokenizerStateInterface
        , private TokenProducer<CreateTokenCallback>
    {
    public:
        InitState(CreateTokenCallback callback)
            : TokenProducer<CreateTokenCallback>(callback)
        {
        }

        TokenizerState consume(const boost::string_view& source, const std::size_t position, FileInfo& fileInfo, Token& token) override
        {
            const char c = source.at(position);
            fileInfo.advanceBy(c);

            if(c == '/')
            {
                token = ResetToken(source, position + 1, TokenType::comment);
                fileInfo.incrementColumn();

                return TokenizerState::FirstSlash;
            }

            if(c == '"')
            {
                token = ResetToken(source, position + 1, TokenType::string_literal);
                fileInfo.incrementColumn();

                return TokenizerState::StringLiteral;
            }

            if(c == '\'')
            {
                token = ResetToken(source, position + 1, TokenType::char_literal);
                fileInfo.incrementColumn();

                return TokenizerState::CharLiteral;
            }

            // 0x01
            if(c == '0')
            {
                token.expand();
                token.type(TokenType::hex_literal);

                return TokenizerState::BeginHexLiteral;
            }

            // 1-9
            if(std::isdigit(c))
            {
                token.expand();
                token.type(TokenType::numeric_literal);

                return TokenizerState::NumericLiteral;
            }

            static const std::string whitespace(" \t\r\n");
            if(whitespace.find_first_of(c) != std::string::npos)
            {
                token.expand();
                token.type(TokenType::whitespace);

                return TokenizerState::Init;
            }

            static const std::string tokenProducers("@=[]{}.:;");
            if(tokenProducers.find_first_of(c) != std::string::npos)
            {
                token.expand();
                token.type(CharToTokenType(c));

                fileInfo = this->produceToken(token, fileInfo);

                token = Token();
                fileInfo.advanceTo(fileInfo);

                return TokenizerState::Init;
            }

            token = ResetToken(source, position);
            return TokenizerState::BeginString;
        }
    };
}}}
