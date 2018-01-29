#include <swizzle/parser/states/StructVectorState.hpp>

#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/ValidateVectorSizeMember.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/utils/CreateMember.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructVectorState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack&, types::TokenStack& tokenStack, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::dot)
        {
            return ParserState::StructVectorNestedOnMember;
        }

        if(type == lexer::TokenType::r_bracket)
        {
            if(types::utils::nodeStackTopIs<ast::nodes::StructField>(nodeStack))
            {
                detail::validateVectorSizeMember(token, nodeStack, tokenStack, context);
                const auto member = types::utils::createMember(token, nodeStack, tokenStack, context, "Expected vector size member information on the token stack, token stack was empty");

                auto& top = static_cast<ast::nodes::StructField&>(*nodeStack.top());
                top.makeVector(member);

                return ParserState::StructEndArrayOrVector;
            }

            throw ParserError("Internal parser error, top of node stack was not ast::nodes::StructField");
        }

        throw SyntaxError("Expected ']' or '.'" , token);
    }
}}}
