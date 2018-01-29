#include <swizzle/parser/states/EnumFieldState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/EnumField.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState EnumFieldState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack&, types::TokenStack&, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::equal)
        {
            return ParserState::EnumFieldEqualRead;
        }

        if(type == lexer::TokenType::comma)
        {
            if(types::utils::nodeStackTopIs<ast::nodes::EnumField>(nodeStack))
            {
                try
                {
                    auto& top = static_cast<ast::nodes::EnumField&>(*nodeStack.top());

                    top.value(context.CurrentEnumValue->assign_field_value(token));
                    context.CurrentEnumValue->increment();

                    nodeStack.pop();
                    return ParserState::EnumStartScope;

                }
                catch(const std::out_of_range&)
                {
                    throw SyntaxError("Enum value overflowed underlying type", token);
                }
            }

            throw ParserError("Internal parser error, top of node stack is not ast::nodes::EnumField");
        }

        throw SyntaxError("Expected ',' or '='", token);
    }
}}}
