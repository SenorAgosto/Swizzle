#pragma once

#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/Token.hpp>

namespace swizzle { namespace lexer {

    // a convenience class for passing FileInfo and Token around together.
    class TokenInfo
    {
    public:
        TokenInfo(const Token& token, const FileInfo& info);
        
        const FileInfo& fileInfo() const { return fileInfo_; }
        FileInfo& fileInfo() { return fileInfo_; }

        void token(const Token& token) { token_ = token; }
        const Token& token() const { return token_; }
        Token& token() { return token_; }

    private:
        FileInfo fileInfo_;
        Token token_;
    };
}}
