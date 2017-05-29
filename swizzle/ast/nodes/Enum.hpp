#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

#include <string>

namespace swizzle { namespace ast { namespace nodes {

    class Enum : public Node
    {
    public:
        Enum(const lexer::TokenInfo& info, const std::string& containingNamespace);

        const lexer::TokenInfo& info() const;
        const lexer::TokenInfo& underlyingTypeInfo() const;

        std::string name() const;

        void underlying(const lexer::TokenInfo& value);
        const lexer::TokenInfo& underlying() const;

    private:
        const lexer::TokenInfo info_;

        lexer::TokenInfo underlyingInfo_;
        lexer::TokenInfo underlyingType_;

        const std::string name_;
    };
}}}
