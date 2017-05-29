#include <swizzle/parser/states/UsingTypeReadState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/TypeAlias.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/CreateType.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

#include <sstream>

namespace swizzle { namespace parser { namespace states {

    ParserState UsingTypeReadState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack& tokenStack, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::colon)
        {
            return ParserState::UsingFirstColon;
        }

        if(type == lexer::TokenType::end_statement)
        {
            const auto info = detail::createType(tokenStack);

            if(detail::nodeStackTopIs<ast::nodes::TypeAlias>(nodeStack))
            {
                auto& top = static_cast<ast::nodes::TypeAlias&>(*nodeStack.top());
                top.existingType(info);

                nodeStack.pop();
            }
            else
            {
                throw ParserError("Internal parser error in UsingTypeReadState, expected TypeAlias on top of NodeStack.");
            }

            return ParserState::TranslationUnitMain;
        }

        std::stringstream ss;
        ss << type;

        throw SyntaxError("Expected ':' or ';', found " + ss.str() + " (" + token.token().to_string() + ")", token);
    }
}}}
