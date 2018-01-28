#include <swizzle/types/utils/CreateNamespace.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/utils/CalculateColumnDifference.hpp>
#include <swizzle/types/utils/StackInvert.hpp>

namespace swizzle { namespace types { namespace utils {

    lexer::TokenInfo createNamespace(TokenStack& tokenStack)
    {
        if(tokenStack.empty())
        {
            throw ParserError("Internal parser error, Token Stack unexpectedly empty.");
        }

        TokenStack stack = types::utils::invert(tokenStack);
        lexer::TokenInfo info = stack.top();
        stack.pop();

        while(!stack.empty())
        {
            const auto& top = stack.top();

            const auto diff = lexer::utils::calculateColumnDifference(info, top);
            info.fileInfo().end() = top.fileInfo().end();
            info.token().expand(diff);

            stack.pop();
        }

        return info;

    }
}}}
