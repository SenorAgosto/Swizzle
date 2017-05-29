#pragma once 
#include <swizzle/parser/ParserStateInterface.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/TokenStack.hpp>

#include <swizzle/parser/detail/CreateNamespace.hpp>

namespace swizzle { namespace parser { namespace states {

    template<class OnNamespaceCallback>
    class NamespaceValueState : public ParserStateInterface
    {
    public:
        NamespaceValueState(OnNamespaceCallback callback)
            : OnNamespace(callback)
        {
        }

        ParserState consume(const lexer::TokenInfo& token, NodeStack&, TokenStack& tokenStack, ParserStateContext& context) override
        {
            const auto type = token.token().type();

            if(type == lexer::TokenType::colon)
            {
                return ParserState::NamespaceFirstColon;
            }

            if(type == lexer::TokenType::end_statement)
            {
                const auto nameSpace = detail::createNamespace(tokenStack);
                context.CurrentNamespace = nameSpace;

                OnNamespace(nameSpace);

                return ParserState::TranslationUnitMain;
            }

            throw SyntaxError("Expected : or ; but found '" + token.token().to_string() + "'", token);
        }

    private:
        OnNamespaceCallback OnNamespace;
    };
}}}
