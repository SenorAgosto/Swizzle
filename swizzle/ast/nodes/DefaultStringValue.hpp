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
        DefaultStringValue(const lexer::TokenInfo& value, const std::string& underlyingType, const std::ptrdiff_t length);

        const lexer::TokenInfo& value() const;
        const std::string& underlying() const;

        std::ptrdiff_t length() const;

        void accept(VisitorInterface& visitor) override;

    private:
        const lexer::TokenInfo value_;
        const std::string underlying_;
        const std::ptrdiff_t length_;
    };
}}}
