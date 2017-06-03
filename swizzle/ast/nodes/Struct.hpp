#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

#include <string>

namespace swizzle { namespace ast {
    class VisitorInterface;
}}

namespace swizzle { namespace ast { namespace nodes {

    class Struct : public Node
    {
    public:
        Struct(const lexer::TokenInfo& info, const lexer::TokenInfo& name, const std::string& containingNamespace);

        const lexer::TokenInfo& info() const;
        const lexer::TokenInfo& nameInfo() const;

        std::string name() const;

        void accept(VisitorInterface& visitor) override;

    private:
        lexer::TokenInfo info_;
        lexer::TokenInfo nameInfo_;

        const std::string name_;
    };
}}}
