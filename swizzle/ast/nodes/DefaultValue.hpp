#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

#include <string>

namespace swizzle { namespace ast { namespace nodes {

    class DefaultValue : public Node
    {
    public:
        DefaultValue(const lexer::TokenInfo& defaultValueInfo, const std::string& underlyingType);

        const lexer::TokenInfo& value() const;
        const std::string& underlying() const;

        void accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Depth depth = Depth::All) override;

    private:
        const lexer::TokenInfo value_;
        const std::string underlying_;
    };
}}}
