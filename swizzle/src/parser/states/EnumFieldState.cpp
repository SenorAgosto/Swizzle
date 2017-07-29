#include <swizzle/parser/states/EnumFieldState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/EnumField.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState EnumFieldState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, NodeStack&, TokenStack&, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::equal)
        {
            return ParserState::EnumFieldEqualRead;
        }

        if(type == lexer::TokenType::comma)
        {
            if(detail::nodeStackTopIs<ast::nodes::EnumField>(nodeStack))
            {
                try
                {
                    auto& top = static_cast<ast::nodes::EnumField&>(*nodeStack.top());

                    top.value(context.CurrentEnumValue.value());
                    context.CurrentEnumValue.increment();

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
