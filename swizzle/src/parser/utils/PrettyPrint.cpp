#include <swizzle/parser/utils/PrettyPrint.hpp>
#include <utility>

namespace swizzle { namespace parser { namespace utils {

    namespace {
        boost::string_view line(const boost::string_view& source, const std::size_t position)
        {
            if(source.empty()) return source;
            
            std::size_t start = position;
            while(source.at(start) != '\n') --start;
            
            std::size_t end = position;
            while(source.at(end) != '\n') ++end;
            
            return source.substr(start, end - start);
        }
        
        // return pair(line, tab_adjustment), where line has tabs replaced with spaces,
        // and tab_adjustment is the number of characters to add to an error for
        // our squiggly lines to match up
        std::pair<std::string, std::size_t> fixup_line(const boost::string_view& sv)
        {
            std::size_t tab_count = 0;
            
            std::string line;
            line.reserve(sv.length());
            
            for(char c : sv)
            {
                if(c == '\t')
                {
                    tab_count++;
                    line.append("    ");
                }
                else
                {
                    line.append(1, c);
                }
            }
            
            // the lexer/parser see a tab as 2 characters, therefore with a tab == 4 spaces, we need
            // to adjust the location by 2 spaces for each tab
            return std::make_pair(line, 2 * tab_count);
        }
    }
    
    // print syntax error message & highlight the line with the error
    void pretty_print(const TokenizerSyntaxError& syntaxError)
    {
        const auto& fileInfo = syntaxError.file_info();
        const auto spaced_line = fixup_line(line(syntaxError.source(), syntaxError.position()));
        
        std::cerr   << "\n"
                    << syntaxError.what() << "\n"
                    << spaced_line.first << "\n";
        
        for(std::size_t i = 0, end = fileInfo.start().column() + spaced_line.second; i < end; ++i)
        {
            std::cout << '~';
        }
        
        std::cout << '^' << std::endl;
    }
    
    void pretty_print(const SyntaxError& syntaxError)
    {
        const auto& token = syntaxError.token();
        const auto spaced_line = fixup_line(token.token().line());
        
        std::cerr   << "\n"
                    << syntaxError.what() << "\n"
                    << spaced_line.first << "\n";
        
        for(std::size_t i = 0, end = token.fileInfo().start().column() + spaced_line.second; i < end; ++i)
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

