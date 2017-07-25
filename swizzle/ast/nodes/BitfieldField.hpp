#pragma once
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

#include <cstdint>

namespace swizzle { namespace ast {
    class VisitorInterface;
}}

namespace swizzle { namespace parser {
    struct ParserStateContext;
}}

namespace swizzle { namespace ast { namespace nodes {

    class BitfieldField : public Node
    {
    public:
        BitfieldField(const lexer::TokenInfo& name, const lexer::TokenInfo& underlyingType);

        const lexer::TokenInfo& name() const;
        const lexer::TokenInfo& underlying() const;

        void beginBit(const lexer::TokenInfo& token, parser::ParserStateContext& context);
        std::size_t beginBit() const;

        void endBit(const lexer::TokenInfo& token, parser::ParserStateContext& context);
        std::size_t endBit() const;

        void accept(VisitorInterface& visitor) override;

    private:
        const lexer::TokenInfo name_;
        const lexer::TokenInfo underlying_;

        lexer::TokenInfo valueInfo_;

        std::intmax_t beginBit_;
        std::intmax_t endBit_;
    };
}}}
