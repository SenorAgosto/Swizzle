#include <swizzle/parser/states/StructVariableBlockCaseValueState.hpp>

#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/ast/nodes/VariableBlockCase.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>
#include <swizzle/types/SetValue.hpp>

namespace swizzle { namespace parser { namespace states {

    namespace {
        ParserState consume_impl(const lexer::TokenInfo& token, NodeStack& nodeStack, NodeStack&, TokenStack&, ParserStateContext& context)
        {
            const auto type = token.token().type();

            if(type == lexer::TokenType::char_literal)
            {
                if(detail::nodeStackTopIs<ast::nodes::VariableBlockCase>(nodeStack))
                {
                    const auto& field = static_cast<ast::nodes::StructField&>(*context.CurrentVariableOnFieldType);
                    if(!field.isArray() && !field.isVector())
                    {
                        auto& blockCase = static_cast<ast::nodes::VariableBlockCase&>(*nodeStack.top());
                        blockCase.value(token);

                        return ParserState::StructVariableBlockCaseValueRead;
                    }

                    throw SyntaxError("variable_block case has hex/numeric/char literal, the field we're variable on must not be an array or vector type", field.type(), token);
                }

                throw ParserError("Internal parser error, top of node stack was not ast::nodes::VariableBlockCase");
            }

            if(type == lexer::TokenType::hex_literal)
            {
                if(detail::nodeStackTopIs<ast::nodes::VariableBlockCase>(nodeStack))
                {
                    const auto& field = static_cast<ast::nodes::StructField&>(*context.CurrentVariableOnFieldType);
                    if(!field.isArray() && !field.isVector())
                    {
                        // ensure field is integer type & assigned value fits.
                        types::setValue(field.type(), token.token().value(), types::isHex, "Error variable_block case value overflows switching type.");

                        auto& blockCase = static_cast<ast::nodes::VariableBlockCase&>(*nodeStack.top());
                        blockCase.value(token);

                        return ParserState::StructVariableBlockCaseValueRead;
                    }

                    throw SyntaxError("variable_block case has hex/numeric/char literal, the field we're variable on must not be an array or vector type", field.type(), token);
                }

                throw ParserError("Internal parser error, top of node stack was not ast::nodes::VariableBlockCase");
            }

            if(type == lexer::TokenType::numeric_literal)
            {
                if(detail::nodeStackTopIs<ast::nodes::VariableBlockCase>(nodeStack))
                {
                    const auto& field = static_cast<ast::nodes::StructField&>(*context.CurrentVariableOnFieldType);
                    if(!field.isArray() && !field.isVector())
                    {
                        // ensure field is integer type & assigned value fits.
                        types::setValue(field.type(), token.token().value(), "Error variable_block case value overflows switching type.");

                        auto& blockCase = static_cast<ast::nodes::VariableBlockCase&>(*nodeStack.top());
                        blockCase.value(token);

                        return ParserState::StructVariableBlockCaseValueRead;
                    }

                    throw SyntaxError("variable_block case has hex/numeric/char literal, the field we're variable on must not be an array or vector type", field.type(), token);
                }

                throw ParserError("Internal parser error, top of node stack was not ast::nodes::VariableBlockCase");
            }

            if(type == lexer::TokenType::string_literal)
            {
                if(detail::nodeStackTopIs<ast::nodes::VariableBlockCase>(nodeStack))
                {
                    const auto& field = static_cast<ast::nodes::StructField&>(*context.CurrentVariableOnFieldType);
                    if(field.isArray())
                    {
                        if((field.arraySize() > 0) && (token.token().value().length() > static_cast<std::size_t>(field.arraySize())))
                        {
                            throw SyntaxError("Case variable overflows switching variable array size.", token);
                        }

                        auto& blockCase = static_cast<ast::nodes::VariableBlockCase&>(*nodeStack.top());
                        blockCase.value(token);

                        return ParserState::StructVariableBlockCaseValueRead;
                    }

                    if(field.isVector())
                    {
                        auto& blockCase = static_cast<ast::nodes::VariableBlockCase&>(*nodeStack.top());
                        blockCase.value(token);

                        return ParserState::StructVariableBlockCaseValueRead;
                    }

                    throw SyntaxError("variable_block case has string_literal, the field we're variable on must be an array or vector type", field.type(), token);
                }
            }

            throw SyntaxError("Expecting literal value (hex, numeric, char, or string)", token);
        }
    }
    
    ParserState StructVariableBlockCaseValueState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, NodeStack& attributeStack, TokenStack& tokenStack, ParserStateContext& context)
    {
        try
        {
            return consume_impl(token, nodeStack, attributeStack, tokenStack, context);
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
