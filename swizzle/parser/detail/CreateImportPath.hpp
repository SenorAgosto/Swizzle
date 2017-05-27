#pragma once 
#include <boost/filesystem/path.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace detail {

    boost::filesystem::path createImportPath(TokenStack& tokenStack);
}}}
