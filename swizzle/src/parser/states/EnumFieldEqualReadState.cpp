#include <swizzle/parser/states/EnumFieldEqualReadState.hpp>

#include <swizzle/ast/nodes/Enum.hpp>
#include <swizzle/ast/nodes/EnumField.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/SetValue.hpp>
#include <swizzle/types/SetValueFromChar.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>
#include <swizzle/types/TokenStack.hpp>

#include <boost/numeric/conversion/cast.hpp>

namespace swizzle { namespace parser { namespace states {

    namespace {
        // implementation is wrapped in a try/catch
        ParserState consumeImpl(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack&, types::TokenStack&, ParserStateContext& context)
        {
            const auto type = token.token().type();

            if(!types::utils::nodeStackTopIs<ast::nodes::EnumField>(nodeStack))
            {
                throw ParserError("Internal parser error, top of stack is not ast::nodes::EnumField");
            }

            auto enumFieldNode = nodeStack.top();
            auto& enumField = static_cast<ast::nodes::EnumField&>(*enumFieldNode);
            enumField.valueInfo(token);
            
            nodeStack.pop();

            if(!types::utils::nodeStackTopIs<ast::nodes::Enum>(nodeStack))
            {
                throw ParserError("Internal parser error, node below top of stack is not ast::nodes::Enum");
            }

            const auto& top = static_cast<ast::nodes::Enum&>(*nodeStack.top());
            const auto underlying = top.underlying();
            nodeStack.push(enumFieldNode);

            if(type == lexer::TokenType::hex_literal)
            {
                context.CurrentEnumValue->value(types::setValue(underlying.token().value(), token, types::isHex, "Encountered unknown enum type"));
                enumField.value(context.CurrentEnumValue->assign_field_value(token));
                context.CurrentEnumValue->increment();
                
                return ParserState::EnumFieldValueRead;
            }

            if(type == lexer::TokenType::numeric_literal)
            {
                context.CurrentEnumValue->value(types::setValue(underlying.token().value(), token, "Encountered unknown enum type"));
                enumField.value(context.CurrentEnumValue->assign_field_value(token));
                context.CurrentEnumValue->increment();

                return ParserState::EnumFieldValueRead;
            }

            if(type == lexer::TokenType::char_literal)
            {
                context.CurrentEnumValue->value(types::setValueFromChar(underlying.token().value(), token));
                enumField.value(context.CurrentEnumValue->assign_field_value(token));
                context.CurrentEnumValue->increment();
                
                return ParserState::EnumFieldValueRead;
            }

            throw SyntaxError("Expected literal value", token);
        }
    }

    ParserState EnumFieldEqualReadState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack& attributeStack, types::TokenStack& tokenStack, ParserStateContext& context)
    {
        try
        {
            return consumeImpl(token, nodeStack, attributeStack, tokenStack, context);
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
