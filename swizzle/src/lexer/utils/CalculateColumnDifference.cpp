#include <swizzle/lexer/utils/CalculateColumnDifference.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

namespace swizzle { namespace lexer { namespace utils {

    std::size_t calculateColumnDifference(const lexer::TokenInfo& info, const lexer::TokenInfo& top)
    {
        if(top.fileInfo().end().column() <= info.fileInfo().end().column())
        {
            throw ParserError("Internal parser error in parser::detail::createType() tokens not on same line.");
        }

        return top.fileInfo().end().column() - info.fileInfo().end().column();
    }

}}}
