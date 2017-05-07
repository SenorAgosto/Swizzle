#pragma once

#include <swizzle/IsKeyword.hpp>

#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/lexer/TokenType.hpp>

namespace swizzle { namespace lexer {

    template<class CreateTokenCallback>
    class TokenProducer
    {
    public:
        TokenProducer(CreateTokenCallback callback)
            : createToken(callback)
        {
        }

        void produceToken(Token& token, const FileInfo& info)
        {
            if(token.empty())
            {
                return;
            }

            if(IsKeyword(token.value()))
            {
                token.type(TokenType::keyword);
            }

            createToken(TokenInfo(token, info));
        }

    private:
        CreateTokenCallback createToken;
    };
}}
