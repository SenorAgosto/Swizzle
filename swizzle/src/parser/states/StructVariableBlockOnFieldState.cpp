#include <swizzle/parser/states/StructVariableBlockOnFieldState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/VariableBlock.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/ValidateVariableBlockSizeMember.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/utils/CreateMember.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructVariableBlockOnFieldState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack&, types::TokenStack& tokenStack, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::dot)
        {
            return ParserState::StructVariableBlockOnNestedField;
        }

        if(type == lexer::TokenType::l_brace)
        {
            context.CurrentVariableOnFieldType = detail::validateVariableBlockSizeMember(token, nodeStack, tokenStack, context);
            const auto member = types::utils::createMember(token, nodeStack, tokenStack, context, "Expected variable block size member information on the token stack, token stack was empty");

            auto& varBlock = static_cast<ast::nodes::VariableBlock&>(*nodeStack.top());
            varBlock.variableOnField(member);

            return ParserState::StructVariableBlockBeginCases;
        }

        throw SyntaxError("Expected '.' or '{'", token);
    }
}}}
