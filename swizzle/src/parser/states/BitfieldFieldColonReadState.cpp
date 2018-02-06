#include <swizzle/parser/states/BitfieldFieldColonReadState.hpp>

#include <swizzle/ast/nodes/Bitfield.hpp>
#include <swizzle/ast/nodes/BitfieldField.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/utils/NodeStackTopIs.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState BitfieldFieldColonReadState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack&, types::TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::numeric_literal)
        {
            if(types::utils::nodeStackTopIs<ast::nodes::BitfieldField>(nodeStack))
            {
                auto bf_node = nodeStack.top();
                nodeStack.pop();
                
                if(types::utils::nodeStackTopIs<ast::nodes::Bitfield>(nodeStack))
                {
                    auto& bf = static_cast<ast::nodes::Bitfield&>(*nodeStack.top());
                    bf.current_bit(token);
                    
                    nodeStack.push(bf_node);
                    
                    auto& bitfield = static_cast<ast::nodes::BitfieldField&>(*nodeStack.top());
                    bitfield.beginBit(bf.current_bit());
                    
                    return ParserState::BitfieldStartPosition;
                }
            
                throw ParserError("Internal parser error, top of node stack was not ast::nodes::Bitfield");
            }

            throw ParserError("Internal parser error, top of node stack was not ast::nodes::BitfieldField");
        }

        throw SyntaxError("Expected bit position (numeric literal)", token);
    }
}}}
