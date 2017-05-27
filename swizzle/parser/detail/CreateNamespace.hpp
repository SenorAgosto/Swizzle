#pragma once
#include <swizzle/parser/TokenStack.hpp>
#include <string>

namespace swizzle { namespace parser { namespace detail {

    std::string createNamespace(TokenStack& tokenStack);
}}}
