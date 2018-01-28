#pragma once 
#include <boost/filesystem/path.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace types { namespace utils {

    boost::filesystem::path createImportPath(TokenStack& tokenStack);
}}}
