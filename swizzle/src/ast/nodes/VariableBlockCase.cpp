#include <swizzle/ast/nodes/VariableBlockCase.hpp>

namespace swizzle { namespace ast { namespace nodes {

    void VariableBlockCase::value(const lexer::TokenInfo& value)
    {
        value_ = value;
    }

    const lexer::TokenInfo& VariableBlockCase::value() const
    {
        return value_;
    }

    void VariableBlockCase::type(const lexer::TokenInfo& type)
    {
        type_ = type;
    }

    lexer::TokenInfo VariableBlockCase::type() const
    {
        return type_;
    }
}}}
