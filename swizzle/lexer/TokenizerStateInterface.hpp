#pragma once 
#include <boost/utility/string_view.hpp>
#include <cstddef>

namespace swizzle { namespace lexer {
    class FileInfo;
    class Token;
    enum class TokenizerState : std::uint8_t;
}}

namespace swizzle { namespace lexer {

    class TokenizerStateInterface
    {
    public:
        virtual ~TokenizerStateInterface(){}
        virtual TokenizerState consume(const boost::string_view& source, const std::size_t position, FileInfo& fileInfo, Token& token) = 0;
    };
}}
