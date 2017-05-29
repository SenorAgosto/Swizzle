#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

namespace swizzle { namespace ast { namespace nodes {

    class Enum : public Node
    {
    public:
        Enum(const lexer::TokenInfo& info, const std::string& containingNamespace);

        const lexer::TokenInfo& info() const;
        std::string name() const;

    private:
        const lexer::TokenInfo info_;
        const std::string name_;
    };
}}}
