#include <swizzle/parser/utils/StackToList.hpp>
#include <utility>

namespace swizzle { namespace parser { namespace utils { namespace stack {

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
}}}}
