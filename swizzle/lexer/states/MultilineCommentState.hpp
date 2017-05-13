#pragma once 
#include <swizzle/lexer/TokenizerStateInterface.hpp>

#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/ResetToken.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/lexer/TokenizerState.hpp>

namespace swizzle { namespace lexer { namespace states {

    template<class CreateTokenCallback>
    class MultilineComment : public TokenizerStateInterface
    {
    public:
        MultilineComment(CreateTokenCallback createToken)
            : createToken_(createToken)
        {
        }

        TokenizerState consume(const boost::string_view& source, const std::size_t position, FileInfo& filePosition, Token& token) override
        {
            const char c = source.at(position);
            if(c == '\n')
            {
                token.expand(source);
                filePosition.incrementLine();

                return TokenizerState::Comment;
            }

            token.expand(source);
            filePosition.incrementColumn();

            return TokenizerState::MultilineComment;
        }

    private:
        CreateTokenCallback createToken_;
    };
}}}
