#pragma once
#include <swizzle/Exceptions.hpp>
#include <iostream>

namespace swizzle { namespace parser { namespace utils {

    // print syntax error message & highlight the line with the error
    void pretty_print(const TokenizerSyntaxError& syntaxError);
    void pretty_print(const SyntaxError& syntaxError);
}}}
