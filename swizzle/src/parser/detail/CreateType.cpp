#include <swizzle/parser/detail/CreateNamespace.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/utils/CalculateColumnDifference.hpp>

namespace swizzle { namespace parser { namespace detail {

    lexer::TokenInfo createType(TokenStack& tokenStack)
    {
        if(tokenStack.empty())
        {
            throw ParserError("Internal parser error, Token Stack unexpectedly empty.");
        }

        TokenStack reverse;
        while(!tokenStack.empty())
        {
            reverse.push(tokenStack.top());
            tokenStack.pop();
        }

        lexer::TokenInfo info = reverse.top();
        reverse.pop();

        while(!reverse.empty())
        {
            const auto& top = reverse.top();

            const auto diff = lexer::utils::calculateColumnDifference(info, top);
            info.fileInfo().end() = top.fileInfo().end();
            info.token().expand(diff);

            reverse.pop();
        }

        return info;
    }
}}}
