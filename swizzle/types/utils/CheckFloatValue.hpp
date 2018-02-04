#pragma once
#include <boost/utility/string_view.hpp>
#include <string>

namespace swizzle { namespace lexer {
    class TokenInfo;
}}

namespace swizzle { namespace types { namespace utils {

    void checkFloatValue(const boost::string_view& underlying, const lexer::TokenInfo& token, const std::string& errorMessage);
}}}
