#include <swizzle/ast/nodes/Bitfield.hpp>

#include <swizzle/ast/AncestorInfo.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/parser/detail/ExtractBitValueFromToken.hpp>

#include <limits>

namespace swizzle { namespace ast { namespace nodes {

    Bitfield::Bitfield(const lexer::TokenInfo& keyword, const lexer::TokenInfo& name, const std::string& containingNamespace)
        : keyword_(keyword)
        , nameDecl_(name)
        , name_(containingNamespace + "::" + nameDecl_.token().to_string())
        , current_bit_value_(std::numeric_limits<std::int16_t>::lowest())
    {
    }

    const lexer::TokenInfo& Bitfield::keyword() const
    {
        return keyword_;
    }

    const lexer::TokenInfo& Bitfield::nameDecl() const
    {
        return nameDecl_;
    }

    std::string Bitfield::name() const
    {
        return name_;
    }

    void Bitfield::underlying(const lexer::TokenInfo& value)
    {
        underlyingType_ = value;
    }

    const lexer::TokenInfo& Bitfield::underlying() const
    {
        return underlyingType_;
    }

    void Bitfield::current_bit(const lexer::TokenInfo& bit_value)
    {
        const std::int16_t bit = parser::detail::extractBitValueFromToken(underlyingType_.token().value(), bit_value);
        
        if(bit < current_bit_value_)
        {
            throw SyntaxError("Bitfield begin bit's value must be greater than previous value", bit_value);
        }
        
        if(bit == current_bit_value_)
        {
            throw SyntaxError("Bitfield begin bit's value must be greater than previous value", bit_value);
        }
        
        current_bit_value_ = bit;
    }

    std::int16_t Bitfield::current_bit() const
    {
        return current_bit_value_;
    }
    
    void Bitfield::accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Depth depth)
    {
        visitor(ancestors, *this);
        
        if(depth == Depth::One) return;
        ancestors.push(*this);
        
        for(auto& child : children())
        {
            auto parent = this;
            child->accept(visitor, ancestors, depth);
        }
        
        ancestors.pop();
    }
}}}

