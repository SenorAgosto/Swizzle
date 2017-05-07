#pragma once 
#include <swizzle/lexer/TokenizerStateInterface.hpp>
#include <swizzle/lexer/TokenizerState.hpp>

namespace swizzle { namespace lexer { namespace states {

    template<class CreateTokenCallback>
    class SyntaxError : public TokenizerStateInterface
    {
    public:
        SyntaxError(CreateTokenCallback createToken)
            : createToken_(createToken)
        {
        }

        TokenizerState consume(const boost::string_view&, const std::size_t, FileInfo&, Token&) override
        {
            return TokenizerState::SyntaxError;
        }

    private:
        CreateTokenCallback createToken_;
    };
}}}
