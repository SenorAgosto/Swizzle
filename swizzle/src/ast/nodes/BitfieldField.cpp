#include <swizzle/ast/nodes/BitfieldField.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/AncestorInfo.hpp>
#include <swizzle/ast/VisitorInterface.hpp>
#include <swizzle/parser/detail/ExtractBitValueFromToken.hpp>
#include <swizzle/parser/ParserStateContext.hpp>

#include <boost/lexical_cast.hpp>
#include <limits>

namespace swizzle { namespace ast { namespace nodes {

    BitfieldField::BitfieldField(const lexer::TokenInfo& name, const lexer::TokenInfo& underlyingType)
        : name_(name)
        , underlying_(underlyingType)
        , beginBit_(0)
        , endBit_(0)
    {
    }

    const lexer::TokenInfo& BitfieldField::name() const
    {
        return name_;
    }

    const lexer::TokenInfo& BitfieldField::underlying() const
    {
        return underlying_;
    }

    void BitfieldField::beginBit(const lexer::TokenInfo& token, parser::ParserStateContext& context)
    {
        const std::intmax_t bit = parser::detail::extractBitValueFromToken(underlying_.token().value(), token);

        if(bit < context.CurrentBitfieldBit)
        {
            throw SyntaxError("Bitfield begin bit's value must be greater than previous value", token);
        }

        if((bit == context.CurrentBitfieldBit) && (context.CurrentBitfieldBit != std::numeric_limits<std::intmax_t>::lowest()))
        {
            throw SyntaxError("Bitfield begin bit's value must be greater than previous value", token);
        }

        context.CurrentBitfieldBit = bit;
        beginBit_ = bit;
        endBit_ = bit;
    }

    std::size_t BitfieldField::beginBit() const
    {
        return beginBit_;
    }

    void BitfieldField::endBit(const lexer::TokenInfo& token, parser::ParserStateContext& context)
    {
        const std::intmax_t bit = parser::detail::extractBitValueFromToken(underlying_.token().value(), token);

        if(bit <= beginBit_)
        {
            throw SyntaxError("Bitfield end bit's value must be greater than begin bit's value", token);
        }

        if(bit < context.CurrentBitfieldBit)
        {
            throw SyntaxError("Bitfield begin bit's value must be greater than previous value", token);
        }

        if((bit == context.CurrentBitfieldBit) && (context.CurrentBitfieldBit != std::numeric_limits<std::intmax_t>::lowest()))
        {
            throw SyntaxError("Bitfield begin bit's value must be greater than previous value", token);
        }

        context.CurrentBitfieldBit = bit;
        endBit_ = bit;
    }

    std::size_t BitfieldField::endBit() const
    {
        return endBit_;
    }

    void BitfieldField::accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Node::Depth depth)
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
