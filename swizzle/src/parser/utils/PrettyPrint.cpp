#include <swizzle/parser/utils/PrettyPrint.hpp>

namespace swizzle { namespace parser { namespace utils {

    // print syntax error message & highlight the line with the error
    void pretty_print(const SyntaxError& syntaxError)
    {
        const auto& token = syntaxError.token();
        std::cerr   << "\n"
                    << syntaxError.what() << "\n"
                    << token.token().line().to_string() << "\n";
        
        for(std::size_t i = 0, end = token.fileInfo().start().column() - 1; i < end; ++i)
        {
            std::cout << '~';
        }
        
        const auto numCarets = token.fileInfo().end().column() - token.fileInfo().start().column();
        for(std::size_t i = 0; i < numCarets; ++i)
        {
            std::cout << '^';
        }
        
        std::cout << std::endl;
    }
}}}

