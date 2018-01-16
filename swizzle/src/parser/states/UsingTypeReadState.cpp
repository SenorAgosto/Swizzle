#include <swizzle/parser/states/UsingTypeReadState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/TypeAlias.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/CreateType.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>
#include <swizzle/parser/utils/NameInTypeCache.hpp>
#include <swizzle/types/IsFloatType.hpp>
#include <swizzle/types/IsIntegerType.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState UsingTypeReadState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, NodeStack&, TokenStack& tokenStack, ParserStateContext& context)
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
                // the aliased type must exist in the TypeCache, or it must be a supported type.
                const auto v = info.token().value();
                if(types::IsIntegerType(v) || types::IsFloatType(v) || utils::NameInTypeCache(context.TypeCache, info.token().value().to_string(), context.CurrentNamespace))
                {
                    auto& top = static_cast<ast::nodes::TypeAlias&>(*nodeStack.top());
                    top.existingType(info);

                    nodeStack.pop();
                    return ParserState::TranslationUnitMain;
                }
                
                throw SyntaxError("Expected defined type, found undefined type", info);
            }
            else
            {
                throw ParserError("Internal parser error in UsingTypeReadState, expected TypeAlias on top of NodeStack.");
            }
        }

        throw SyntaxError("Expected ':' or ';'", token);
    }
}}}
