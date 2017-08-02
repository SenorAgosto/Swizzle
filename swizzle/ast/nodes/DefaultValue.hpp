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

        const lexer::TokenInfo& value() const;
        const lexer::TokenInfo& underlying() const;

        void accept(VisitorInterface& visitor) override;

    private:
        const lexer::TokenInfo value_;
        const lexer::TokenInfo underlying_;
    };
}}}
