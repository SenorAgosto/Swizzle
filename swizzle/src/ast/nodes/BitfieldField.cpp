#include <swizzle/ast/nodes/BitfieldField.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/ast/VisitorInterface.hpp>
#include <swizzle/parser/ParserStateContext.hpp>

namespace swizzle { namespace ast { namespace nodes {

        BitfieldField::BitfieldField(const lexer::TokenInfo& name, const lexer::TokenInfo& underlyingType)
            : name_(name)
            , underlying_(underlyingType)
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

        void BitfieldField::beginBit(const lexer::TokenInfo& token, const std::size_t bit, parser::ParserStateContext& context)
        {
            if(bit < context.CurrentBitfieldBit)
            {
                throw SyntaxError("Bitfield begin bit's value must be greater than previous value", token);
            }

            if((bit == context.CurrentBitfieldBit) && (context.CurrentBitfieldBit != 0))
            {
                throw SyntaxError("Bitfield begin bit's value must be greater than previous value", token);
            }

            // TODO: ensure bit is within range supported by the underlying type

            context.CurrentBitfieldBit = bit;
            beginBit_ = bit;
            endBit_ = bit;
        }

        std::size_t BitfieldField::beginBit() const
        {
            return beginBit_;
        }

        void BitfieldField::endBit(const lexer::TokenInfo& token, const std::size_t bit, parser::ParserStateContext& context)
        {
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

            // TODO: ensure bit is within the underlying type

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
