#include <swizzle/parser/detail/CreateNamespace.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/utils/CalculateColumnDifference.hpp>
#include <swizzle/parser/utils/StackInvert.hpp>

namespace swizzle { namespace parser { namespace detail {

    lexer::TokenInfo createType(TokenStack& tokenStack)
    {
        if(tokenStack.empty())
        {
            throw ParserError("Internal parser error, Token Stack unexpectedly empty.");
        }

        TokenStack stack = utils::stack::invert(tokenStack);

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
