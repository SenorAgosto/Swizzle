#include <swizzle/ast/nodes/Bitfield.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    Bitfield::Bitfield(const lexer::TokenInfo& bitfieldInfo, const lexer::TokenInfo& name, const std::string& containingNamespace)
        : bitfieldInfo_(bitfieldInfo)
        , nameInfo_(name)
        , name_(containingNamespace + "::" + nameInfo_.token().to_string())
    {
    }

    const lexer::TokenInfo& Bitfield::bitfieldInfo() const
    {
        return bitfieldInfo_;
    }

    const lexer::TokenInfo& Bitfield::nameInfo() const
    {
        return nameInfo_;
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

    void Bitfield::accept(VisitorInterface& visitor)
    {
        visitor(*this);

        for(auto& child : children())
        {
            child->accept(visitor);
        }
    }
}}}
