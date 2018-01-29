#pragma once
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/types/EnumValue.hpp>

#include <cstdint>
#include <memory>

namespace swizzle { namespace ast { namespace nodes {

    class EnumField : public Node
    {
    public:
        EnumField(const lexer::TokenInfo& name, const lexer::TokenInfo& underlyingType);

        const lexer::TokenInfo& name() const;
        const lexer::TokenInfo& underlying() const;

        void valueInfo(const lexer::TokenInfo& val);
        const lexer::TokenInfo& valueInfo() const;

        void value(const std::uint64_t value);
        std::uint64_t value() const;

        void accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Node::Depth depth = Node::Depth::All) override;

    private:
        const lexer::TokenInfo name_;
        const lexer::TokenInfo underlying_;

        lexer::TokenInfo valueInfo_;
        std::uint64_t value_;
    };
}}}
