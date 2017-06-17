#include <swizzle/parser/utils/StackInvert.hpp>
#include <swizzle/parser/TokenStack.hpp>

#include <utility>

namespace swizzle { namespace parser { namespace utils { namespace stack {

    TokenStack invert(TokenStack& stack)
    {
        TokenStack inverted;

        while(!stack.empty())
        {
            inverted.emplace(std::move(stack.top()));
            stack.pop();
        }

        return inverted;
    }
}}}}
