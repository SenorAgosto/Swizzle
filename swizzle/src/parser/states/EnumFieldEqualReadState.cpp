#include <swizzle/parser/states/EnumFieldEqualReadState.hpp>

#include <swizzle/ast/nodes/Enum.hpp>
#include <swizzle/ast/nodes/EnumField.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>
#include <swizzle/types/SetValue.hpp>
#include <swizzle/types/SetValueFromChar.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState EnumFieldEqualReadState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack&, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(!detail::nodeStackTopIs<ast::nodes::EnumField>(nodeStack))
        {
            throw ParserError("Internal parser error, top of stack is not ast::nodes::EnumField");
        }

        auto enumFieldNode = nodeStack.top();
        auto& enumField = static_cast<ast::nodes::EnumField&>(*enumFieldNode);
        nodeStack.pop();

        if(!detail::nodeStackTopIs<ast::nodes::Enum>(nodeStack))
        {
            throw ParserError("Internal parser error, node below top of stack is not ast::nodes::Enum");
        }

        const auto& top = static_cast<ast::nodes::Enum&>(*nodeStack.top());
        const auto underlying = top.underlying();
        nodeStack.push(enumFieldNode);

        if(type == lexer::TokenType::hex_literal)
        {
            static const bool isHex = true;
            enumField.value(types::setValue<isHex>(underlying.token().value(), token.token().value()));
            context.CurrentEnumValue = enumField.value();

            return ParserState::EnumFieldValueRead;
        }

        if(type == lexer::TokenType::numeric_literal)
        {
            static const bool isNotHex = false;
            enumField.value(types::setValue<isNotHex>(underlying.token().value(), token.token().value()));
            context.CurrentEnumValue = enumField.value();

            return ParserState::EnumFieldValueRead;
        }

        if(type == lexer::TokenType::char_literal)
        {
            auto trimValue = token.token().value();
            trimValue.remove_prefix(1); // remove leading '
            trimValue.remove_suffix(1); // remove trailing '

            enumField.value(types::setValueFromChar(underlying.token().value(), trimValue));
            context.CurrentEnumValue = enumField.value();
            
            return ParserState::EnumFieldValueRead;
        }

        throw SyntaxError("Expected literal value", token);
    }
}}}
