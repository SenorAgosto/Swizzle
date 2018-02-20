#pragma once
#include <swizzle/parser/ParserStateInterface.hpp>

namespace swizzle { namespace parser { namespace states {

    class StructBaseColonReadState : public ParserStateInterface
    {
    public:
        ParserState consume(const lexer::TokenInfo& token, types::NodeStack& nodeStack, types::NodeStack& attributeStack, types::TokenStack& tokenStack, ParserStateContext& context) override;
        
    private:
        bool equalRead_ = false;
    };
}}}

