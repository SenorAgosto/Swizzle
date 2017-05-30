#pragma once 
#include <swizzle/lexer/TokenizerStateInterface.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/ResetToken.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/lexer/TokenProducer.hpp>
#include <swizzle/lexer/TokenizerState.hpp>

#include <cctype>

namespace swizzle { namespace lexer { namespace states {

    template<class CreateTokenCallback>
    class FloatingPointLiteralState
        : public TokenizerStateInterface
        , private TokenProducer<CreateTokenCallback>
    {
    public:
        FloatingPointLiteralState(CreateTokenCallback createToken)
            : TokenProducer<CreateTokenCallback>(createToken)
        {
        }

        TokenizerState consume(const boost::string_view& source, const std::size_t position, FileInfo& fileInfo, Token& token) override
        {
            const char c = source.at(position);

            if(c == '.')
            {
                // if the last digit of the token is ., then this is a range
                if(token.value().back() == '.')
                {
                    token.contract();   // remove trailing .
                    token.type(TokenType::numeric_literal);

                    fileInfo = this->produceToken(token, fileInfo);

                    token = ResetToken(source, position - 1, TokenType::dot);
                    fileInfo = this->produceToken(token, fileInfo);

                    token = ResetToken(source, position, TokenType::dot);
                    fileInfo = this->produceToken(token, fileInfo);

                    return TokenizerState::Init;
                }

                throw TokenizerError("Floating point values can only have one decimal point");
            }

            if(std::isdigit(c))
            {
                token.expand();
                return TokenizerState::FloatingPointLiteral;
            }

            static const std::string whitespace(" \t\r\n");
            if(whitespace.find_first_of(c) != std::string::npos)
            {
                fileInfo = this->produceToken(token, fileInfo);
                token = ResetToken(source, position, TokenType::whitespace);

                return TokenizerState::Init;
            }

            static const std::string tokenProducers("]}:;,");
            if(tokenProducers.find_first_of(c) != std::string::npos)
            {
                fileInfo = this->produceToken(token, fileInfo);
                token = ResetToken(source, position, CharToTokenType(c));

                fileInfo = this->produceToken(token, fileInfo);
                token = ResetToken(source, position + 1);

                return TokenizerState::Init;
            }

            throw TokenizerError("Unexpected character following numeric literal: '" + std::string(1, c) + "'");
        }
    };
}}}
