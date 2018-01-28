#pragma once 
#include <swizzle/parser/ParserStateInterface.hpp>


namespace swizzle { namespace parser { namespace states {

    class NamespaceValueState : public ParserStateInterface
    {
    public:
        ParserState consume(const lexer::TokenInfo& token, types::NodeStack&, types::NodeStack& attributeStack, types::TokenStack& tokenStack, ParserStateContext& context) override;
    };
}}}
