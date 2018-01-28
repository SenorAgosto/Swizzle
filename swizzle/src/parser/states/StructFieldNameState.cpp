#include <swizzle/parser/states/StructFieldNameState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/utils/AttachAttributes.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructFieldNameState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack& attributeStack, types::TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::end_statement)
        {
            if(types::utils::nodeStackTopIs<ast::nodes::StructField>(nodeStack))
            {
                types::utils::attachAttributes(attributeStack, nodeStack.top());
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
