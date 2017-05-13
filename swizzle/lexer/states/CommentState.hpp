#pragma once 
#include <swizzle/lexer/TokenizerStateInterface.hpp>

#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/ResetToken.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/lexer/TokenProducer.hpp>
#include <swizzle/lexer/TokenizerState.hpp>

namespace swizzle { namespace lexer { namespace states {

    template<class CreateTokenCallback>
    class CommentState
        : public TokenizerStateInterface
        , private TokenProducer<CreateTokenCallback>
    {
    public:
        CommentState(CreateTokenCallback callback)
            : TokenProducer<CreateTokenCallback>(callback)
        {
        }

        TokenizerState consume(const boost::string_view& source, const std::size_t position, FileInfo& filePosition, Token& token) override
        {
            const char c = source.at(position);
            if(c == '\\')
            {
                token.type(TokenType::multiline_comment);
                filePosition.incrementColumn();

                return TokenizerState::MultilineComment;
            }

            if(c == '\n')
            {
                this->produceToken(token, filePosition);
                token = ResetToken(source, position);

                filePosition.incrementLine();

                return TokenizerState::Init;
            }

            token.expand(source);
            filePosition.incrementColumn();

            return TokenizerState::Comment;
        }
    };
}}}
