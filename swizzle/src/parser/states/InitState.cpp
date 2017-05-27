#include <swizzle/parser/states/InitState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/lexer/TokenType.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/TokenStack.hpp>

#include <swizzle/ast/AbstractTreeVertexVisitorInterface.hpp>
#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/nodes/MultilineComment.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState InitState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, TokenStack&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::comment)
        {
            ast::Node::smartptr comment = new ast::nodes::Comment(token);
            nodeStack.push(comment);

            return ParserState::Init;
        }

        if(type == lexer::TokenType::multiline_comment)
        {
            ast::Node::smartptr comment = new ast::nodes::MultilineComment(token);
            nodeStack.push(comment);

            return ParserState::Init;
        }

        const auto& value = token.token().value();
        if((type == lexer::TokenType::keyword) && ("import" == value))
        {
            return ParserState::StartImport;
        }

        if((type == lexer::TokenType::keyword) && ("namespace" == value))
        {
            return ParserState::StartNamespace;
        }


        throw SyntaxError("Expected comment, import statement, or namespace declaration, found: '" + value.to_string() + "'", token);
    }
}}}
