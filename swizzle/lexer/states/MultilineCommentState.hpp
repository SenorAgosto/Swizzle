#pragma once 
#include <swizzle/lexer/TokenizerStateInterface.hpp>

#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/ResetToken.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/lexer/TokenizerState.hpp>

namespace swizzle { namespace lexer { namespace states {

    template<class CreateTokenCallback>
    class MultilineCommentState : public TokenizerStateInterface
    {
    public:
        MultilineCommentState(CreateTokenCallback)
        {
        }

        TokenizerState consume(const boost::string_view& source, const std::size_t position, FileInfo& fileInfo, Token& token) override
        {
            const char c = source.at(position);
            if(c == '\n')
            {
                token.expand();
                fileInfo.incrementLine();

                return TokenizerState::Comment;
            }

            token.expand();
            fileInfo.incrementColumn();

            return TokenizerState::MultilineComment;
        }
    };
}}}
