#include <swizzle/backend/SwizzleService.hpp>

namespace swizzle { namespace backend {

    SwizzleService::SwizzleService(const int argc, char** argv)
        : argc_(argc)
        , argv_(argv)
    {
    }

    SwizzleService::SwizzleService(const SwizzleService& other)
        : argc_(other.argc_)
        , argv_(other.argv_)
    {
    }
    
    SwizzleService::SwizzleService(SwizzleService&& other)
        : argc_(std::move(other.argc_))
        , argv_(std::move(other.argv_))
    {
    }
    
    SwizzleService& SwizzleService::operator=(const SwizzleService& other)
    {
        argc_ = other.argc_;
        argv_ = other.argv_;
        
        return *this;
    }
    
    SwizzleService& SwizzleService::operator=(SwizzleService&& other)
    {
        argc_ = std::move(other.argc_);
        argv_ = std::move(other.argv_);
        
        return *this;
    }
    
    std::pair<const int, char const * const *> SwizzleService::program_options() const
    {
        return std::make_pair(argc_, argv_);
    }
}}

