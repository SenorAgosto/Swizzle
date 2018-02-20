#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

namespace swizzle { namespace ast { namespace nodes {

    class TypeAlias : public Node
    {
    public:
        TypeAlias(const lexer::TokenInfo& keyword, const lexer::TokenInfo& newType);

        const lexer::TokenInfo& keyword() const;
        const lexer::TokenInfo& aliasedType() const;

        void existingType(const lexer::TokenInfo& info);
        const lexer::TokenInfo& existingType() const;

        void accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Node::Depth depth = Node::Depth::All) override;
        
    private:
        const lexer::TokenInfo keyword_;

        const lexer::TokenInfo aliasedType_;
        lexer::TokenInfo existingType_;
    };
}}}
