#include <swizzle/parser/states/StructVariableBlockCaseValueState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/ast/nodes/VariableBlockCase.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructVariableBlockCaseValueState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack&, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if((type == lexer::TokenType::hex_literal) ||
           (type == lexer::TokenType::numeric_literal) ||
           (type == lexer::TokenType::char_literal))
        {
            if(detail::nodeStackTopIs<ast::nodes::VariableBlockCase>(nodeStack))
            {
                auto& blockCase = static_cast<ast::nodes::VariableBlockCase&>(*nodeStack.top());
                blockCase.value(token);

                const auto& field = static_cast<ast::nodes::StructField&>(*context.CurrentVariableOnFieldType);
                if(!field.isArray() && !field.isVector())
                {
                    // TODO: ensure the value fits the underlying type.
                    return ParserState::StructVariableBlockCaseValueRead;
                }

                throw SyntaxError("variable_block case has hex/numeric/char literal, the field we're variable on must not be an array or vector type", field.type(), token.fileInfo());
            }

            throw ParserError("Internal parser error, top of node stack was not ast::nodes::VariableBlockCase");
        }

        if(type == lexer::TokenType::string_literal)
        {
            if(detail::nodeStackTopIs<ast::nodes::VariableBlockCase>(nodeStack))
            {
                auto& blockCase = static_cast<ast::nodes::VariableBlockCase&>(*nodeStack.top());
                blockCase.value(token);

                const auto& field = static_cast<ast::nodes::StructField&>(*context.CurrentVariableOnFieldType);
                if(field.isArray() || field.isVector())
                {
                    return ParserState::StructVariableBlockCaseValueRead;
                }

                throw SyntaxError("variable_block case has string_literal, the field we're variable on must be an array or vector type", field.type(), token.fileInfo());
            }
        }

        throw SyntaxError("Expecting literal value (hex, numeric, char, or string)", token);
    }
}}}
