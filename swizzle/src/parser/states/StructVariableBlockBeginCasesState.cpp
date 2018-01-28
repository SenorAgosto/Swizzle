#include <swizzle/parser/states/StructVariableBlockBeginCasesState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/Matcher.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/ast/nodes/VariableBlockCase.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/utils/AppendNode.hpp>
#include <swizzle/types/TokenStack.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState StructVariableBlockBeginCasesState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack&, types::TokenStack&, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::comment)
        {
            types::utils::appendNode<ast::nodes::Comment>(nodeStack, token);
            return ParserState::StructVariableBlockBeginCases;
        }

        if(type == lexer::TokenType::multiline_comment)
        {
            types::utils::appendNode<ast::nodes::MultilineComment>(nodeStack, token);
            return ParserState::StructVariableBlockBeginCases;
        }

        if((type == lexer::TokenType::keyword) && (token.token().value() == "case"))
        {
            auto node = types::utils::appendNode<ast::nodes::VariableBlockCase>(nodeStack);
            nodeStack.push(node);

            return ParserState::StructVariableBlockCaseValue;
        }

        if(type == lexer::TokenType::r_brace)
        {
            const auto& top = nodeStack.top();

            auto hasNonCommentChildren = ast::Matcher().hasChildNotOf<ast::nodes::Comment, ast::nodes::MultilineComment>();
            if(!hasNonCommentChildren(top))
            {
                throw SyntaxError("Expected variable block cases", " empty variable block", token);
            }

            nodeStack.pop();
            return ParserState::StructStartScope;
        }

        throw SyntaxError("Expected 'case' or '}'", token);
    }    
}}}
