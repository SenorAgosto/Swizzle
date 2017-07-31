#include <swizzle/parser/states/StructEndArrayOrVectorState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructEndArrayOrVectorState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, NodeStack&, TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::string)
        {
            if(detail::nodeStackTopIs<ast::nodes::StructField>(nodeStack))
            {
                auto& top = static_cast<ast::nodes::StructField&>(*nodeStack.top());
                top.name(token);

                return ParserState::StructFieldName;
            }
        }

        throw SyntaxError("Expected field name", token);
    }
}}}
