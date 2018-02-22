#pragma once
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

namespace swizzle { namespace ast { namespace nodes {

    class VariableBlockCase : public Node
    {
    public:
        void value(const lexer::TokenInfo& value);  // set the case value
        const lexer::TokenInfo& value() const;      // @return the case value

        void type(const lexer::TokenInfo& type);
        lexer::TokenInfo type() const;

        void accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Depth depth = Depth::All) override;
        
    private:
        lexer::TokenInfo value_;
        lexer::TokenInfo type_;
    };
}}}
