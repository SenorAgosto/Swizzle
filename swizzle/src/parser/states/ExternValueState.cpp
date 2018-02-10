#include <swizzle/parser/states/ExternValueState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/Extern.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/utils/AppendNode.hpp>
#include <swizzle/types/utils/ClearTokenStack.hpp>
#include <swizzle/types/utils/CreateType.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState ExternValueState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack&, types::TokenStack& tokenStack, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::colon)
        {
            return ParserState::ExternFirstColon;
        }

        if(type == lexer::TokenType::end_statement)
        {
            auto externType = types::utils::createType(tokenStack);
            types::utils::clear(tokenStack);

            types::utils::appendNode<ast::nodes::Extern>(nodeStack, externType);
            
            const auto externName = externType.token().value().to_string();
            context.SymbolTable.insert(externName, types::SymbolInfo(externName, types::SymbolType::Extern, nullptr));
            
            return ParserState::Init;
        }

        throw SyntaxError("Expected ';' or ':'", token);
    }

}}}
