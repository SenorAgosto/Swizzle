#include <swizzle/types/utils/StackInvert.hpp>
#include <utility>

namespace swizzle { namespace types { namespace utils {

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
}}}
