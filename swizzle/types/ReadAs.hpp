#pragma once
#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/types/SafeStringStream.hpp>

#include <boost/utility/string_view.hpp>

namespace swizzle { namespace types {

    template<class T>
    T readAs(const lexer::TokenInfo& token)
    {
        safe_istringstream is(token);

        T t = 0;
        is >> t;

        return t;
    }

    template<class T>
    T readAsHex(const lexer::TokenInfo& token)
    {
        safe_istringstream is(token);

        T t = 0;
        is >> hex_stream >> t;

        return t;
    }
}}
