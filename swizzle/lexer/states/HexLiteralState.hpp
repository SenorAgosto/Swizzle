#pragma once 
#include <swizzle/lexer/TokenizerStateInterface.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/ResetToken.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenType.hpp>
#include <swizzle/lexer/TokenProducer.hpp>
#include <swizzle/lexer/TokenizerState.hpp>

#include <cctype>

namespace swizzle { namespace lexer { namespace states {

    template<class CreateTokenCallback>
    class HexLiteralState
        : public TokenizerStateInterface
        , private TokenProducer<CreateTokenCallback>
    {
    public:
        HexLiteralState(CreateTokenCallback createToken)
            : TokenProducer<CreateTokenCallback>(createToken)
        {
        }

        TokenizerState consume(const boost::string_view& source, const std::size_t position, FileInfo& fileInfo, Token& token) override
        {
            const char c = source.at(position);

            if(std::isxdigit(c))
            {
                token.expand();
                fileInfo.advanceBy(c);

                return TokenizerState::HexLiteral;
            }

            static const std::string whitespace(" \t\r\n");
            if(whitespace.find_first_of(c) != std::string::npos)
            {
                fileInfo = this->produceToken(token, fileInfo);

                token.expand();
                token.type(TokenType::whitespace);

                return TokenizerState::Init;
            }

            static const std::string tokenProducers("];,");
            if(tokenProducers.find_first_of(c) != std::string::npos)
            {
                fileInfo = this->produceToken(token, fileInfo);

                token = ResetToken(source, position, CharToTokenType(c));
                token.expand();

                fileInfo = this->produceToken(token, fileInfo);

                token = ResetToken(source, position);
                fileInfo.advanceTo(fileInfo);

                return TokenizerState::Init;
            }

            throw TokenizerError("Unexpected character following hex literal: '" + std::string(1, c) + "'");
        }
    };
}}}
