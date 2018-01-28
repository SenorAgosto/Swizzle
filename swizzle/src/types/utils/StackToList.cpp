#include <swizzle/types/utils/StackToList.hpp>
#include <utility>

namespace swizzle { namespace types { namespace utils {

    TokenList to_list(TokenStack& stack)
    {
        TokenList list;

        while(!stack.empty())
        {
            list.emplace_back(std::move(stack.top()));
            stack.pop();
        }

        return list;
    }
}}}
