#include <swizzle/parser/states/StructEndArrayOrVectorState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/StructField.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/TokenStack.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructEndArrayOrVectorState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack&, types::TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::string)
        {
            if(types::utils::nodeStackTopIs<ast::nodes::StructField>(nodeStack))
            {
                auto& top = static_cast<ast::nodes::StructField&>(*nodeStack.top());
                top.name(token);

                return ParserState::StructFieldName;
            }
        }

        throw SyntaxError("Expected field name", token);
    }
}}}
