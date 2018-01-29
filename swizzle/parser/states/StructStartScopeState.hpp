#pragma once 
#include <swizzle/parser/ParserStateInterface.hpp>

namespace swizzle { namespace parser { namespace states {

    class StructStartScopeState : public ParserStateInterface
    {
    public:
        ParserState consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack& attributeStack, types::TokenStack& tokenStack, ParserStateContext& context) override;

    private:
        bool equalRead_ = false;    // track if we've read an equals in a key/value attribute
    };
}}}
