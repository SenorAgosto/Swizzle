#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

#include <string>
#include <unordered_set>

namespace swizzle { namespace ast { namespace nodes {

    class Struct : public Node
    {
    public:
        Struct(const lexer::TokenInfo& keyword, const lexer::TokenInfo& name, const std::string& containingNamespace);
        Struct(const Struct& str);
        
        const lexer::TokenInfo& keyword() const;
        const lexer::TokenInfo& nameDecl() const;

        std::string name() const;

        void allocate_label(const lexer::TokenInfo& token);
        
    public:
        void accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Node::Depth depth = Node::Depth::All) override;

    private:
        lexer::TokenInfo keyword_;
        lexer::TokenInfo nameDecl_;

        const std::string name_;
        std::unordered_set<std::string> fieldLabels_;
    };
}}}
