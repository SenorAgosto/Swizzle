#include <swizzle/parser/utils/ClearTokenStack.hpp>

namespace swizzle { namespace parser { namespace utils {

    void clear(TokenStack& tokenStack)
    {
        while(!tokenStack.empty())
        {
            tokenStack.pop();
        }
    }
}}}
