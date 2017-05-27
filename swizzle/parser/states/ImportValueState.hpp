#pragma once 
#include <swizzle/parser/ParserStateInterface.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/NodeStack.hpp>
#include <swizzle/parser/TokenStack.hpp>

#include <swizzle/parser/detail/CreateImportPath.hpp>
#include <swizzle/parser/detail/ValidateImportPath.hpp>

namespace swizzle { namespace parser { namespace states {

    // when an import statement is completed OnImportCallback
    // will be called so the definitions of the imported types
    // can be parsed before continuing parsing the current file.
    template<class OnImportCallback>
    class ImportValueState : public ParserStateInterface
    {
    public:
        ImportValueState(OnImportCallback callback)
            : OnImport(callback)
        {
        }

        ParserState consume(const lexer::TokenInfo& token, NodeStack&, TokenStack& tokenStack) override
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

                OnImport(import);
                return ParserState::Init;
            }

            throw SyntaxError("Expected ; or :, found '" + token.token().to_string() + "'", token);
        }

    private:
        OnImportCallback OnImport;
    };
}}}
