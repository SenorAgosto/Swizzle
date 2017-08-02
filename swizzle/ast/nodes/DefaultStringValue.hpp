#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

#include <cstddef>
#include <string>

namespace swizzle { namespace ast {
    class VisitorInterface;
}}

namespace swizzle { namespace ast { namespace nodes {

    class DefaultStringValue : public Node
    {
    public:
        DefaultStringValue(const lexer::TokenInfo& defaultValueInfo, const lexer::TokenInfo& underlyingType, const std::ptrdiff_t length);

        const lexer::TokenInfo& info() const;
        const lexer::TokenInfo& underlying() const;

        void value(const lexer::TokenInfo& valueToken);
        const lexer::TokenInfo& value() const;

        std::ptrdiff_t length() const;

        void accept(VisitorInterface& visitor) override;

    private:
        const lexer::TokenInfo info_;
        const lexer::TokenInfo underlying_;
        const std::ptrdiff_t length_;

        lexer::TokenInfo value_;    // string literal value
    };
}}}
