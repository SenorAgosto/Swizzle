#include <swizzle/types/utils/CreateMember.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/Matcher.hpp>
#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/lexer/utils/CalculateColumnDifference.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/utils/StackInvert.hpp>
#include <swizzle/types/TokenStack.hpp>

#include <utility>

namespace swizzle { namespace types { namespace utils {

    lexer::TokenInfo createMember(const lexer::TokenInfo& token, types::NodeStack&, types::TokenStack& tokenStack, const parser::ParserStateContext&, const std::string& onEmptyTokenStack)
    {
        if(tokenStack.empty())
        {
            throw SyntaxError(onEmptyTokenStack, token);
        }

        auto stack = types::utils::invert(tokenStack);
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
