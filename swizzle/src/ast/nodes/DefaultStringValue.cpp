#include <swizzle/ast/nodes/DefaultStringValue.hpp>

#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    DefaultStringValue::DefaultStringValue(const lexer::TokenInfo& defaultValueInfo, const lexer::TokenInfo& underlyingType, const std::ptrdiff_t length)
        : info_(defaultValueInfo)
        , underlying_(underlyingType)
        , length_(length)
    {
    }

    const lexer::TokenInfo& DefaultStringValue::info() const
    {
        return info_;
    }

    const lexer::TokenInfo& DefaultStringValue::underlying() const
    {
        return underlying_;
    }

    void DefaultStringValue::value(const lexer::TokenInfo& valueToken)
    {
        value_ = valueToken;
    }

    const lexer::TokenInfo& DefaultStringValue::value() const
    {
        return value_;
    }

    std::ptrdiff_t DefaultStringValue::length() const
    {
        return length_;
    }

    void DefaultStringValue::accept(VisitorInterface& visitor)
    {
        visitor(*this);

        for(auto& child : children())
        {
            child->accept(visitor);
        }
    }
}}}
