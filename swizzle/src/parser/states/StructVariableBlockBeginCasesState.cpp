#include <swizzle/parser/states/StructVariableBlockBeginCasesState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/ast/nodes/VariableBlockCase.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/parser/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructVariableBlockBeginCasesState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::comment)
        {
            detail::appendNode<ast::nodes::Comment>(nodeStack, token);
            return ParserState::StructVariableBlockBeginCases;
        }

        if(type == lexer::TokenType::multiline_comment)
        {
            detail::appendNode<ast::nodes::MultilineComment>(nodeStack, token);
            return ParserState::StructVariableBlockBeginCases;
        }

        if((type == lexer::TokenType::keyword) && (token.token().value() == "case"))
        {
            detail::appendNode<ast::nodes::VariableBlockCase>(nodeStack);
            return ParserState::StructVariableBlockCaseValue;
        }

        if(type == lexer::TokenType::r_brace)
        {
            const auto& top = nodeStack.top();
            if(top->empty())
            {
                throw SyntaxError("Expected variable block cases", " empty variable block", token.fileInfo());
            }

            nodeStack.pop();
            return ParserState::StructStartScope;
        }

        throw SyntaxError("Expected 'case' or '}'", token);
    }    
}}}
