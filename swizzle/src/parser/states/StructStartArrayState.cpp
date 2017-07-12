#include <swizzle/parser/states/StructStartArrayState.hpp>

#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructStartArrayState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack& tokenStack, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::numeric_literal)
        {
            if(detail::nodeStackTopIs<ast::nodes::StructField>(nodeStack))
            {
                auto& top = static_cast<ast::nodes::StructField&>(*nodeStack.top());
                top.makeArray(token);

                return ParserState::StructArray;
            }

            throw ParserError("Internal parser error, top of node stack was not ast::nodes::StructField");
        }

        if(type == lexer::TokenType::string)
        {
            tokenStack.push(token);
            return ParserState::StructVector;
        }

        throw SyntaxError("Expected array size or vector size member name", token);
    }
}}}
