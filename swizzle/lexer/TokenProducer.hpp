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
            const LineInfo newStart = info.end();
            const LineInfo newEnd = LineInfo(newStart.line(), newStart.column() + 1);
            const FileInfo newInfo(info.filename(), newStart, newEnd);

            if(token.empty())
            {
                return newInfo;
            }

            if(IsKeyword(token.value()))
            {
                token.type(TokenType::keyword);
            }

            createToken(TokenInfo(token, info));

            return newInfo;
        }

    private:
        CreateTokenCallback createToken;
    };
}}
