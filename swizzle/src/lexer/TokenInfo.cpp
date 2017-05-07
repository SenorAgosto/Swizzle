#include <swizzle/lexer/TokenInfo.hpp>

namespace swizzle { namespace lexer {

    TokenInfo::TokenInfo(const Token& token, const FileInfo& info)
        : fileInfo_(info)
        , token_(token)
    {
    }
}}
