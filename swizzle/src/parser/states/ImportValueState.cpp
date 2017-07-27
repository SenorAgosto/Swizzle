#include <swizzle/parser/states/ImportValueState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/Import.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/TokenStack.hpp>

#include <swizzle/parser/detail/AppendNode.hpp>
#include <swizzle/parser/detail/CreateImportPath.hpp>
#include <swizzle/parser/detail/ValidateImportPath.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState ImportValueState::consume(const lexer::TokenInfo& token, NodeStack& nodeStack, NodeStack&, TokenStack& tokenStack, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::colon)
        {
            return ParserState::ImportFirstColon;
        }

        if(type == lexer::TokenType::end_statement)
        {
            const boost::filesystem::path import = detail::createImportPath(tokenStack);
            detail::validateImportPath(import);

            detail::appendNode<ast::nodes::Import>(nodeStack, token, import);

            return ParserState::Init;
        }

        throw SyntaxError("Expected ';' or ':'", token);
    }

}}}
