#pragma once 
#include <swizzle/lexer/TokenizerStateInterface.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/ResetToken.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/lexer/TokenizerState.hpp>

namespace swizzle { namespace lexer { namespace states {

    template<class CreateTokenCallback>
    class FirstSlashState : public TokenizerStateInterface
    {
    public:
        FirstSlashState(CreateTokenCallback)
        {
        }

        TokenizerState consume(const boost::string_view& source, const std::size_t position, FileInfo&, Token& token) override
        {
            const char c = source.at(position);
            if(c == '/')
            {
                token.expand();
                return TokenizerState::Comment;
            }

            // TODO: improve error reporting to print the line and draw squiggles
            throw TokenizerError("Expected '/' to begin comment, found " + std::string(1, c));
        }
    };
}}}
