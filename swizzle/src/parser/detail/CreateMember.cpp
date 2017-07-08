#include <swizzle/parser/detail/CreateMember.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/Matcher.hpp>
#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/lexer/utils/CalculateColumnDifference.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>
#include <swizzle/parser/utils/StackInvert.hpp>

#include <utility>

namespace swizzle { namespace parser { namespace detail {

    lexer::TokenInfo createMember(const lexer::TokenInfo& token, NodeStack&, TokenStack& tokenStack, const ParserStateContext&, const std::string& onEmptyTokenStack)
    {
        if(tokenStack.empty())
        {
            throw SyntaxError(onEmptyTokenStack, token);
        }

        TokenStack stack = utils::stack::invert(tokenStack);
        lexer::TokenInfo info = stack.top();
        stack.pop();

        while(!stack.empty())
        {
            const auto& top = stack.top();

            const auto diff = lexer::utils::calculateColumnDifference(info, top);
            info.fileInfo().end() = top.fileInfo().end();
            info.token().expand(diff);

            stack.pop();
        }

        return info;
    }

}}}
