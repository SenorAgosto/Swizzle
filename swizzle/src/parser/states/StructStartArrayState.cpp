#include <swizzle/parser/states/StructStartArrayState.hpp>

#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructStartArrayState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack&, types::TokenStack& tokenStack, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::numeric_literal)
        {
            if(types::utils::nodeStackTopIs<ast::nodes::StructField>(nodeStack))
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
