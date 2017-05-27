#include <swizzle/parser/detail/CreateNamespace.hpp>
#include <swizzle/Exceptions.hpp>

namespace swizzle { namespace parser { namespace detail {

    std::string createNamespace(TokenStack& tokenStack)
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

        std::string nameSpace;
        while(!reverse.empty())
        {
            nameSpace += reverse.top().token().to_string() + "::";
            reverse.pop();
        }

        nameSpace.resize(nameSpace.size() - 2);
        return nameSpace;
    }
}}}
