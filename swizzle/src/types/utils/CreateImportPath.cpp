#include <swizzle/types/utils/CreateImportPath.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/types/utils/StackInvert.hpp>

namespace swizzle { namespace types { namespace utils {

    boost::filesystem::path createImportPath(TokenStack& tokenStack)
    {
        if(tokenStack.empty())
        {
            throw ParserError("Internal parser error, Token Stack unexpectedly empty.");
        }

        TokenStack stack = types::utils::invert(tokenStack);

        boost::filesystem::path import;
        while(!stack.empty())
        {
            import /= stack.top().token().to_string();
            stack.pop();
        }

        const auto leaf = import.leaf();
        import.remove_leaf();
        import /= leaf.string() + ".swizzle";

        return import;
    }
}}}
