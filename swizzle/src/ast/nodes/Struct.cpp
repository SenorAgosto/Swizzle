#include <swizzle/ast/nodes/Struct.hpp>

#include <swizzle/ast/AncestorInfo.hpp>
#include <swizzle/ast/VisitorInterface.hpp>
#include <swizzle/Exceptions.hpp>

namespace swizzle { namespace ast { namespace nodes {

    Struct::Struct(const lexer::TokenInfo& info, const lexer::TokenInfo& name, const std::string& containingNamespace)
        : info_(info)
        , nameInfo_(name)
        , name_(containingNamespace + "::" + name.token().to_string())
    {
    }

    const lexer::TokenInfo& Struct::info() const
    {
        return info_;
    }

    const lexer::TokenInfo& Struct::nameInfo() const
    {
        return nameInfo_;
    }

    std::string Struct::name() const
    {
        return name_;
    }

    void Struct::allocate_label(const lexer::TokenInfo& token)
    {
        const auto label = token.token().value().to_string();
        
        const auto iter = fieldLabels_.find(label);
        if(iter != fieldLabels_.cend())
        {
            throw SyntaxError("Duplicate field label value assigned.", token);
        }
        
        fieldLabels_.insert(label);
    }

    // virtual
    void Struct::accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Node::Depth depth)
    {
        visitor(ancestors, *this);
        
        if(depth == Depth::One) return;
        ancestors.push(*this);
        
        for(auto& child : children())
        {
            child->accept(visitor, ancestors, depth);
        }
        
        ancestors.pop();
    }
}}}
