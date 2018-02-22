#pragma once
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

#include <cstdint>

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

        void beginBit(const std::int16_t bit);
        std::int16_t beginBit() const;

        void endBit(const std::int16_t bit);
        std::int16_t endBit() const;

    public:
        void accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Depth depth = Depth::All) override;

    private:
        const lexer::TokenInfo name_;
        const lexer::TokenInfo underlying_;

        lexer::TokenInfo valueInfo_;

        std::int16_t beginBit_;
        std::int16_t endBit_;
    };
}}}
