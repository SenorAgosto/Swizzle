#include <swizzle/ast/nodes/DefaultValue.hpp>

#include <swizzle/ast/VisitorInterface.hpp>
#include <swizzle/types/SetValue.hpp>

namespace swizzle { namespace ast { namespace nodes {

    DefaultValue::DefaultValue(const lexer::TokenInfo& defaultValueInfo, const lexer::TokenInfo& underlyingType)
        : info_(defaultValueInfo)
        , underlying_(underlyingType)
    {
    }

    const lexer::TokenInfo& DefaultValue::info() const
    {
        return info_;
    }

    const lexer::TokenInfo& DefaultValue::underlying() const
    {
        return underlying_;
    }

    void DefaultValue::value(const lexer::TokenInfo& valueToken)
    {
        value_ = valueToken;
    }

    const lexer::TokenInfo& DefaultValue::value() const
    {
        return value_;
    }

    void DefaultValue::accept(VisitorInterface& visitor)
    {
        visitor(*this);

        for(auto& child : children())
        {
            child->accept(visitor);
        }
    }

}}}
