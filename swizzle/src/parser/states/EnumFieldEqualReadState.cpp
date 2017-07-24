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

#include <boost/numeric/conversion/cast.hpp>

namespace swizzle { namespace parser { namespace states {

    namespace {
        // implementation is wrapped in a try/catch
        ParserState consumeImpl(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack&, ParserStateContext& context)
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
                enumField.value(types::setValue(underlying.token().value(), token.token().value(), types::isHex));

                context.CurrentEnumValue = enumField.value();
                context.CurrentEnumValue.increment();

                return ParserState::EnumFieldValueRead;
            }

            if(type == lexer::TokenType::numeric_literal)
            {
                enumField.value(types::setValue(underlying.token().value(), token.token().value()));

                context.CurrentEnumValue = enumField.value();
                context.CurrentEnumValue.increment();

                return ParserState::EnumFieldValueRead;
            }

            if(type == lexer::TokenType::char_literal)
            {
                auto trimValue = token.token().value();
                trimValue.remove_prefix(1); // remove leading '
                trimValue.remove_suffix(1); // remove trailing '

                enumField.value(types::setValueFromChar(underlying.token().value(), trimValue));

                context.CurrentEnumValue = enumField.value();
                context.CurrentEnumValue.increment();
                
                return ParserState::EnumFieldValueRead;
            }

            throw SyntaxError("Expected literal value", token);
        }
    }

    ParserState EnumFieldEqualReadState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack& tokenStack, ParserStateContext& context)
    {
        try
        {
            return consumeImpl(token, nodeStack, tokenStack, context);
        }
        catch(const StreamInputCausesOverflow&)
        {
            throw SyntaxError("Enum field value overflows underlying type", token);
        }
        catch(const StreamInputCausesUnderflow& valueError)
        {
            throw SyntaxError("Enum field value underflows undelying type", token);
        }
        catch(const InvalidStreamInput&)
        {
            throw SyntaxError("Enum field value contais an invalid character", token);
        }
    }
}}}
