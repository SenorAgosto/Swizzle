#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

#include <cstdint>
#include <string>

namespace swizzle { namespace ast { namespace nodes {

    class Bitfield : public Node
    {
    public:
        Bitfield(const lexer::TokenInfo& bitfieldInfo, const lexer::TokenInfo& name, const std::string& containingNamespace);

        const lexer::TokenInfo& bitfieldInfo() const;
        const lexer::TokenInfo& nameInfo() const;
        const lexer::TokenInfo& underlyingTypeInfo() const;

        std::string name() const;

        void underlying(const lexer::TokenInfo& value);
        const lexer::TokenInfo& underlying() const;

        void current_bit(const lexer::TokenInfo& bit_value);
        std::int16_t current_bit() const;
        
    public:
        void accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Node::Depth depth = Node::Depth::All) override;

    private:
        const lexer::TokenInfo bitfieldInfo_;   // bitfield keyword
        const lexer::TokenInfo nameInfo_;       // bitfield name

        lexer::TokenInfo underlyingInfo_;
        lexer::TokenInfo underlyingType_;

        const std::string name_;
        std::int16_t current_bit_value_;
    };
}}}
