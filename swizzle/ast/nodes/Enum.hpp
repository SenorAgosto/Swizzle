#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

#include <string>

namespace swizzle { namespace ast {
    class VisitorInterface;
}}

namespace swizzle { namespace ast { namespace nodes {

    class Enum : public Node
    {
    public:
        Enum(const lexer::TokenInfo& enumInfo, const lexer::TokenInfo& name, const std::string& containingNamespace);

        const lexer::TokenInfo& enumInfo() const;
        const lexer::TokenInfo& nameInfo() const;

        std::string name() const;

        void underlying(const lexer::TokenInfo& value);
        const lexer::TokenInfo& underlying() const;

        void accept(VisitorInterface& visitor, Node& parent, const Node::Depth depth = Node::Depth::All) override;

    private:
        const lexer::TokenInfo enumInfo_;   // enum keyword
        const lexer::TokenInfo nameInfo_;   // enum name

        lexer::TokenInfo underlyingInfo_;
        lexer::TokenInfo underlyingType_;

        const std::string name_;
    };
}}}
