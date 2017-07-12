#include <swizzle/parser/states/ExternValueState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/Extern.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/TokenStack.hpp>

#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/detail/CreateType.hpp>
#include <swizzle/parser/utils/ClearTokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState ExternValueState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack& tokenStack, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::colon)
        {
            return ParserState::ExternFirstColon;
        }

        if(type == lexer::TokenType::end_statement)
        {
            auto externType = detail::createType(tokenStack);
            utils::clear(tokenStack);

            detail::appendNode<ast::nodes::Extern>(nodeStack, externType);
            return ParserState::Init;
        }

        throw SyntaxError("Expected ';' or ':'", token);
    }

}}}
