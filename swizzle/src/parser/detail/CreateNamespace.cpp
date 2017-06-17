#include <swizzle/parser/detail/CreateNamespace.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/utils/StackInvert.hpp>

namespace swizzle { namespace parser { namespace detail {

    std::string createNamespace(TokenStack& tokenStack)
    {
        if(tokenStack.empty())
        {
            throw ParserError("Internal parser error, Token Stack unexpectedly empty.");
        }

        std::string nameSpace;
        TokenStack stack = utils::stack::invert(tokenStack);

        while(!stack.empty())
        {
            nameSpace += stack.top().token().to_string() + "::";
            stack.pop();
        }

        nameSpace.resize(nameSpace.size() - 2);
        return nameSpace;
    }
}}}
