#include <swizzle/types/utils/ClearTokenStack.hpp>

namespace swizzle { namespace types { namespace utils {

    void clear(TokenStack& tokenStack)
    {
        while(!tokenStack.empty())
        {
            tokenStack.pop();
        }
    }
}}}
