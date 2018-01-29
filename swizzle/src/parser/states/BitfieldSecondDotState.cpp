#include <swizzle/parser/states/BitfieldSecondDotState.hpp>

#include <swizzle/ast/nodes/BitfieldField.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState BitfieldSecondDotState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack&, types::TokenStack&, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::numeric_literal)
        {
            if(types::utils::nodeStackTopIs<ast::nodes::BitfieldField>(nodeStack))
            {
                auto& top = static_cast<ast::nodes::BitfieldField&>(*nodeStack.top());
                top.endBit(token, context);

                return ParserState::BitfieldEndPosition;
            }

            throw ParserError("Internal parser error, top of node stack is not ast::nodes::BitfieldField");
        }

        throw SyntaxError("Expected bit position (numeric literal)", token);
    }
}}}
