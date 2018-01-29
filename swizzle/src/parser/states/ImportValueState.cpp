#include <swizzle/parser/states/ImportValueState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/Import.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/utils/AppendNode.hpp>
#include <swizzle/types/TokenStack.hpp>

#include <swizzle/types/utils/CreateImportPath.hpp>
#include <swizzle/parser/detail/ValidateImportPath.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState ImportValueState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack&, types::TokenStack& tokenStack, ParserStateContext&)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::colon)
        {
            return ParserState::ImportFirstColon;
        }

        if(type == lexer::TokenType::end_statement)
        {
            const boost::filesystem::path import = types::utils::createImportPath(tokenStack);
            detail::validateImportPath(import);

            types::utils::appendNode<ast::nodes::Import>(nodeStack, token, import);

            return ParserState::Init;
        }

        throw SyntaxError("Expected ';' or ':'", token);
    }

}}}
