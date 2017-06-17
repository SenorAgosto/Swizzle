#include <swizzle/parser/detail/CreateImportPath.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/utils/StackInvert.hpp>

namespace swizzle { namespace parser { namespace detail {

    boost::filesystem::path createImportPath(TokenStack& tokenStack)
    {
        if(tokenStack.empty())
        {
            throw ParserError("Internal parser error, Token Stack unexpectedly empty.");
        }

        TokenStack stack = utils::stack::invert(tokenStack);

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
