#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

#include <string>

namespace swizzle { namespace ast {
    class VisitorInterface;
}}

namespace swizzle { namespace ast { namespace nodes {

    class DefaultValue : public Node
    {
    public:
        DefaultValue(const lexer::TokenInfo& defaultValueInfo, const lexer::TokenInfo& underlyingType);

        const lexer::TokenInfo& info() const;
        const lexer::TokenInfo& underlying() const;

        void value(const lexer::TokenInfo& valueToken);
        const lexer::TokenInfo& value() const;

        void accept(VisitorInterface& visitor) override;

    private:
        const lexer::TokenInfo info_;
        const lexer::TokenInfo underlying_;

        lexer::TokenInfo value_;    // literal value
    };
}}}
