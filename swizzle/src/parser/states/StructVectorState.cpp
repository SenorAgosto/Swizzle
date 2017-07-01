#include <swizzle/parser/states/StructVectorState.hpp>

#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/CreateMember.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/detail/ValidateVectorSizeMember.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructVectorState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack& tokenStack, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::dot)
        {
            return ParserState::StructVectorNestedOnMember;
        }

        if(type == lexer::TokenType::r_bracket)
        {
            if(detail::nodeStackTopIs<ast::nodes::StructField>(nodeStack))
            {
                detail::validateVectorSizeMember(token, nodeStack, tokenStack, context);
                const auto member = detail::createMember(token, nodeStack, tokenStack, context);

                auto& top = static_cast<ast::nodes::StructField&>(*nodeStack.top());
                top.makeVector(member);

                nodeStack.pop();
                return ParserState::StructStartScope;
            }
            else
            {
                throw ParserError("Internal parser error, top of node stack was not ast::nodes::StructField");
            }
        }

        throw SyntaxError("Expected ']' or '.'" , token);
    }
}}}
