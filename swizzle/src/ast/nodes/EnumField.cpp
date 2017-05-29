#include <swizzle/ast/nodes/EnumField.hpp>
#include <swizzle/ast/AbstractTreeVertexVisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    EnumField::EnumField(const lexer::TokenInfo& info, const lexer::TokenInfo& underlyingType)
    {
    }

    const lexer::TokenInfo& EnumField::name() const
    {
    }

    const lexer::TokenInfo& EnumField::underlyingType() const
    {
    }

    void EnumField::value(const lexer::TokenInfo& val)
    {
        value_ = val;
    }

    const lexer::TokenInfo& EnumField::value() const
    {
        return value_;
    }
}}}
