#pragma once 

#pragma clang diagnostic push 
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wsign-compare"

#include <boost/numeric/safe/safe_integer.hpp>

namespace swizzle { namespace platform {

    template<class T>
    using safe = boost::numeric::safe<T>;
}}

#pragma clang diagnostic pop
