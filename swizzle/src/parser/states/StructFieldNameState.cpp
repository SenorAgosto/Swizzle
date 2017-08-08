#include <swizzle/parser/states/StructFieldNameState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/AttachAttributes.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructFieldNameState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, NodeStack& attributeStack, TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::end_statement)
        {
            if(detail::nodeStackTopIs<ast::nodes::StructField>(nodeStack))
            {
                detail::attachAttributes(attributeStack, nodeStack.top());
                nodeStack.pop();

                return ParserState::StructStartScope;
            }

            throw ParserError("Internal parser error, top of node stack is not ast::nodes::StructField");
        }

        if(type == lexer::TokenType::equal)
        {
            return ParserState::StructFieldEqualRead;
        }

        throw SyntaxError("Expected ';'", token);
    }
}}}
