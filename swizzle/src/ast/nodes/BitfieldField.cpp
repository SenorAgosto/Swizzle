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
        , beginBit_(std::numeric_limits<std::int16_t>::lowest())
        , endBit_(std::numeric_limits<std::int16_t>::lowest())
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

    void BitfieldField::beginBit(const std::int16_t bit)
    {
        beginBit_ = bit;
        endBit_ = bit;
    }

    std::int16_t BitfieldField::beginBit() const
    {
        return beginBit_;
    }

    void BitfieldField::endBit(const std::int16_t bit)
    {
        endBit_ = bit;
    }

    std::int16_t BitfieldField::endBit() const
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
