#pragma once
#include <utility>

namespace swizzle { namespace backend {

    class SwizzleService
    {
    public:
        SwizzleService(const int argc, char** arg);
        
        SwizzleService(const SwizzleService&);
        SwizzleService(SwizzleService&&);
        SwizzleService& operator=(const SwizzleService&);
        SwizzleService& operator=(SwizzleService&&);
        
        std::pair<const int, char const * const*> program_options() const;
        
    private:
        int argc_;
        char ** argv_;
    };
}}
