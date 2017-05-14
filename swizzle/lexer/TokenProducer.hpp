#pragma once

#include <swizzle/IsKeyword.hpp>

#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/LineInfo.hpp>
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

        FileInfo produceToken(Token& token, const FileInfo& info)
        {
            FileInfo newInfo(info.filename(), info.end(), info.end());
            newInfo.advanceBy(token);

            if(token.empty())
            {
                return newInfo;
            }

            if(IsKeyword(token.value()))
            {
                token.type(TokenType::keyword);
            }

            createToken(TokenInfo(token, newInfo));
            return newInfo;
        }

    private:
        CreateTokenCallback createToken;
    };
}}
