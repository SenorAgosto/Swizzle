#include <swizzle/parser/states/BitfieldFieldColonReadState.hpp>

#include <swizzle/ast/nodes/BitfieldField.hpp>
#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/NodeStackTopIs.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState BitfieldFieldColonReadState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, NodeStack&, TokenStack&, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::numeric_literal)
        {
            if(detail::nodeStackTopIs<ast::nodes::BitfieldField>(nodeStack))
            {
                auto& bitfield = static_cast<ast::nodes::BitfieldField&>(*nodeStack.top());
                bitfield.beginBit(token, context);

                return ParserState::BitfieldStartPosition;
            }

            throw ParserError("Internal parser error, top of node stack was not ast::nodes::BitfieldField");
        }

        throw SyntaxError("Expected bit position (numeric literal)", token);
    }
}}}
