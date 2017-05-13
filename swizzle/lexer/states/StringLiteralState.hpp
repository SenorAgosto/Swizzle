#pragma once 
#include <swizzle/lexer/TokenizerStateInterface.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/ResetToken.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenProducer.hpp>
#include <swizzle/lexer/TokenType.hpp>
#include <swizzle/lexer/TokenizerState.hpp>

namespace swizzle { namespace lexer { namespace states {

    template<class CreateTokenCallback>
    class StringLiteral
        : public TokenizerStateInterface
        , TokenProducer<CreateTokenCallback>
    {
    public:
        StringLiteral(CreateTokenCallback createToken)
            : TokenProducer<CreateTokenCallback>(createToken)
        {
        }

        TokenizerState consume(const boost::string_view& source, const std::size_t position, FileInfo& filePosition, Token& token) override
        {
            const char c = source.at(position);
            if(c == '"')
            {
                this->produceToken(token, filePosition);
                token = ResetToken(source, position);

                return TokenizerState::Init;
            }

            if(c == '\\')
            {
                token.expand(source);
                filePosition.advanceBy(c);

                return TokenizerState::EscapedCharInStringLiteral;
            }

            token.expand(source);
            filePosition.advanceBy(c);

            return TokenizerState::StringLiteral;
        }
    };
}}}
