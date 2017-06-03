#include <swizzle/ast/nodes/BitfieldField.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/VisitorInterface.hpp>
#include <swizzle/parser/ParserStateContext.hpp>

#include <boost/lexical_cast.hpp>

namespace swizzle { namespace ast { namespace nodes {

        namespace {
            std::size_t extractBitValueFromToken(const lexer::TokenInfo& underlyingInfo, const lexer::TokenInfo& info)
            {
                const auto& underlying = underlyingInfo.token();
                try
                {
                    if(underlying.value() == "u8")
                    {
                        return boost::lexical_cast<std::uint8_t>(info.token().value());
                    }

                    if(underlying.value() == "u16")
                    {
                        return boost::lexical_cast<std::uint16_t>(info.token().value());
                    }

                    if(underlying.value() == "u32")
                    {
                        return boost::lexical_cast<std::uint32_t>(info.token().value());
                    }

                    if(underlying.value() == "u64")
                    {
                        return boost::lexical_cast<std::uint64_t>(info.token().value());
                    }
                }
                catch(const boost::bad_lexical_cast&)
                {
                    throw SyntaxError("Bitfield bit value exceeds range supported by underlying type (" + underlying.to_string() + ")", info);
                }

                throw ParserError("Internal parser error, unexpected underlying type found parsing bitfield");
            }

        }

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
            const std::size_t bit = extractBitValueFromToken(underlying_, token);

            if(bit < context.CurrentBitfieldBit)
            {
                throw SyntaxError("Bitfield begin bit's value must be greater than previous value", token);
            }

            if((bit == context.CurrentBitfieldBit) && (context.CurrentBitfieldBit != 0))
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
            const std::size_t bit = extractBitValueFromToken(underlying_, token);

            if(bit <= beginBit_)
            {
                throw SyntaxError("Bitfield end bit's value must be greater than begin bit's value", token);
            }

            if(bit < context.CurrentBitfieldBit)
            {
                throw SyntaxError("Bitfield begin bit's value must be greater than previous value", token);
            }

            if((bit == context.CurrentBitfieldBit) && (context.CurrentBitfieldBit != 0))
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

        void BitfieldField::accept(VisitorInterface& visitor)
        {
            visitor(*this);

            for(auto& child : children())
            {
                child->accept(visitor);
            }
        }
}}}
