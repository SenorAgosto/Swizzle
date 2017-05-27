#include <swizzle/parser/detail/CreateImportPath.hpp>
#include <swizzle/Exceptions.hpp>

namespace swizzle { namespace parser { namespace detail {

    boost::filesystem::path createImportPath(TokenStack& tokenStack)
    {
        if(tokenStack.empty())
        {
            throw ParserError("Internal parser error, Token Stack unexpectedly empty.");
        }

        TokenStack reverse;
        while(!tokenStack.empty())
        {
            reverse.push(tokenStack.top());
            tokenStack.pop();
        }

        boost::filesystem::path import;
        while(!reverse.empty())
        {
            import /= reverse.top().token().to_string();
            reverse.pop();
        }

        const auto leaf = import.leaf();
        import.remove_leaf();
        import /= leaf.string() + ".swizzle";

        return import;
    }
}}}
