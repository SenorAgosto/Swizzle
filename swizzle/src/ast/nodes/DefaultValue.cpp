#include <swizzle/ast/nodes/DefaultValue.hpp>

#include <swizzle/ast/VisitorInterface.hpp>
#include <swizzle/types/SetValue.hpp>

namespace swizzle { namespace ast { namespace nodes {

    DefaultValue::DefaultValue(const lexer::TokenInfo& value, const lexer::TokenInfo& underlyingType)
        : value_(value)
        , underlying_(underlyingType)
    {
    }

    const lexer::TokenInfo& DefaultValue::value() const
    {
        return value_;
    }

    const lexer::TokenInfo& DefaultValue::underlying() const
    {
        return underlying_;
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
