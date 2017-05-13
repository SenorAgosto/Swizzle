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

        TokenizerState consume(const boost::string_view& source, const std::size_t position, FileInfo& filePosition, Token& token) override
        {
            const char c = source.at(position);
            if(c == '/')
            {
                this->produceToken(token, filePosition);

                token = ResetToken(source, position, TokenType::comment);
                filePosition.incrementColumn();

                return TokenizerState::FirstSlash;
            }

            if(c == '"')
            {
                this->produceToken(token, filePosition);

                token = ResetToken(source, position, TokenType::string_literal);
                filePosition.incrementColumn();

                return TokenizerState::StringLiteral;
            }

            if(c == '\'')
            {
                this->produceToken(token, filePosition);

                token = ResetToken(source, position, TokenType::char_literal);
                filePosition.incrementColumn();

                return TokenizerState::CharLiteral;
            }

            static const std::string tokenProducers("@=[]{}.;:");
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

            static const std::string whitespace(" \t\r\n");
            if(whitespace.find_first_of(c) != std::string::npos)
            {
                this->produceToken(token, filePosition);
                token = ResetToken(source, position);

                return TokenizerState::Init;
            }

            token.expand(source);
            filePosition.advanceBy(c);

            return TokenizerState::BeginString;
        }
    };
}}}
