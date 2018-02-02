#include <swizzle/types/utils/StartsWith.hpp>

namespace swizzle { namespace types { namespace utils {

    bool starts_with(const std::string& corpus, const std::string& pattern)
    {
        return corpus.substr(0, pattern.length()) == pattern;
    }
}}}
