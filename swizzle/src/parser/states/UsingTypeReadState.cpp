#include <swizzle/parser/states/UsingTypeReadState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/TypeAlias.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/IsFloatType.hpp>
#include <swizzle/types/IsIntegerType.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/utils/CreateType.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState UsingTypeReadState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack&, types::TokenStack& tokenStack, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::colon)
        {
            return ParserState::UsingFirstColon;
        }

        if(type == lexer::TokenType::end_statement)
        {
            const auto info = types::utils::createType(tokenStack);

            if(types::utils::nodeStackTopIs<ast::nodes::TypeAlias>(nodeStack))
            {
                const auto v = info.token().value();
                if(types::IsIntegerType(v) || types::IsFloatType(v) || context.SymbolTable.contains(context.CurrentNamespace, info.token().value().to_string()))
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
