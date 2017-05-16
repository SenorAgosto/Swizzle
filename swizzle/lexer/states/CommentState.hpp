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

        TokenizerState consume(const boost::string_view& source, const std::size_t position, FileInfo& fileInfo, Token& token) override
        {
            const char c = source.at(position);
            if(c == '\\')
            {
                token.expand();
                token.type(TokenType::multiline_comment);

                return TokenizerState::MultilineComment;
            }

            if(c == '\n')
            {
                fileInfo = this->produceToken(token, fileInfo);

                token = ResetToken(source, position);
                fileInfo.incrementLine();   // manually increment line count

                return TokenizerState::Init;
            }

            token.expand();
            return TokenizerState::Comment;
        }
    };
}}}
