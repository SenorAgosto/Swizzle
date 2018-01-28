#include <swizzle/parser/states/NamespaceValueState.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/nodes/Namespace.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/ParserStateContext.hpp>
#include <swizzle/types/NodeStack.hpp>
#include <swizzle/types/utils/AppendNode.hpp>
#include <swizzle/types/TokenStack.hpp>

#include <swizzle/types/utils/CreateNamespace.hpp>

namespace swizzle { namespace parser { namespace states {

    ParserState NamespaceValueState::consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack&, types::TokenStack& tokenStack, ParserStateContext& context)
    {
        const auto type = token.token().type();

        if(type == lexer::TokenType::colon)
        {
            return ParserState::NamespaceFirstColon;
        }

        if(type == lexer::TokenType::end_statement)
        {
            const auto nameSpace = types::utils::createNamespace(tokenStack);

            types::utils::appendNode<ast::nodes::Namespace>(nodeStack, nameSpace);
            context.CurrentNamespace = nameSpace.token().value().to_string();

            return ParserState::TranslationUnitMain;
        }

        throw SyntaxError("Expected : or ; but found '" + token.token().to_string() + "'", token);
    }

}}}
