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

    lexer::TokenInfo createMember(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack& tokenStack, const ParserStateContext& context)
    {
        if(tokenStack.empty())
        {
            throw SyntaxError("Expected vector size member information on the token stack, token stack was empty", token);
        }

        if(nodeStack.empty())
        {
            throw SyntaxError("Node stack empty, expected top of node stack to be ast::nodes::StructField", token);
        }

        auto isStructField = ast::Matcher().isTypeOf<ast::nodes::StructField>();
        if(!isStructField(nodeStack.top()))
        {
            throw SyntaxError("Expected top of node stack to be ast::nodes::StructField", token);
        }

        const auto field = nodeStack.top();
        nodeStack.pop();

        auto isStruct = ast::Matcher().isTypeOf<ast::nodes::Struct>();
        if(!isStruct(nodeStack.top()))
        {
            throw SyntaxError("Expected node below top of node stack to be ast::nodes::Struct", token);
        }

        const auto structure = nodeStack.top();
        nodeStack.emplace(std::move(field));

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
