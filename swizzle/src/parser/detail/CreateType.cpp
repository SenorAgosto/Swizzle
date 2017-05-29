#include <swizzle/parser/detail/CreateNamespace.hpp>
#include <swizzle/Exceptions.hpp>

namespace swizzle { namespace parser { namespace detail {

    namespace {
        std::size_t calculateColumnDifference(const lexer::TokenInfo& info, const lexer::TokenInfo& top)
        {
            if(top.fileInfo().end().column() <= info.fileInfo().end().column())
            {
                throw ParserError("Internal parser error in parser::detail::createType() tokens not on same line.");
            }

            return top.fileInfo().end().column() - info.fileInfo().end().column();
        }
    }

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

            const auto diff = calculateColumnDifference(info, top);
            info.fileInfo().end() = top.fileInfo().end();
            info.token().expand(diff);

            reverse.pop();
        }

        return info;
    }
}}}
