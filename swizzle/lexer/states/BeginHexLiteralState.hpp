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
    class BeginHexLiteralState
        : public TokenizerStateInterface
        , private TokenProducer<CreateTokenCallback>
    {
    public:
        BeginHexLiteralState(CreateTokenCallback createToken)
            : TokenProducer<CreateTokenCallback>(createToken)
        {
        }

        TokenizerState consume(const boost::string_view& source, const std::size_t position, FileInfo& filePosition, Token& token) override
        {
            const char c = source.at(position);

            if(std::isdigit(c))
            {
                token.expand(source);
                filePosition.advanceBy(c);

                return TokenizerState::NumericLiteral;
            }

            if(c == 'x')
            {
                token.expand(source);
                token.type(TokenType::hex_literal);

                filePosition.advanceBy(c);
                return TokenizerState::HexLiteral;
            }

            static const std::string whitespace(" \t\r\n");
            if(whitespace.find_first_of(c) != std::string::npos)
            {
                this->produceToken(token, filePosition);

                token = ResetToken(source, position);
                filePosition.advanceBy(c);

                return TokenizerState::Init;
            }

            static const std::string tokenProducers("]}.;,");
            if(tokenProducers.find_first_of(c) != std::string::npos)
            {
                this->produceToken(token, filePosition);

                LineInfo newStart = LineInfo(filePosition.end().line(), filePosition.end().column());
                LineInfo newEnd = newStart;
                newEnd.incrementColumn();

                filePosition = FileInfo(filePosition.filename(), newStart, newEnd);

                token = ResetToken(source, position, CharToTokenType(c));
                token.expand(source);

                this->produceToken(token, filePosition);
                token = ResetToken(source, position);

                return TokenizerState::Init;
            }

            throw TokenizerError("Unexpected character following numeric literal: '" + std::string(1, c) + "'");
        }
    };
}}}
