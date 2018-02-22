#include <swizzle/ast/nodes/Struct.hpp>

#include <swizzle/ast/AncestorInfo.hpp>
#include <swizzle/ast/VisitorInterface.hpp>
#include <swizzle/Exceptions.hpp>

namespace swizzle { namespace ast { namespace nodes {

    Struct::Struct(const lexer::TokenInfo& keyword, const lexer::TokenInfo& name, const std::string& containingNamespace)
        : keyword_(keyword)
        , nameDecl_(name)
        , name_(containingNamespace + "::" + name.token().to_string())
    {
    }

    Struct::Struct(const Struct& str)
        : keyword_(str.keyword())
        , nameDecl_(str.nameDecl())
        , name_(str.name())
    {
    }
    
    const lexer::TokenInfo& Struct::keyword() const
    {
        return keyword_;
    }

    const lexer::TokenInfo& Struct::nameDecl() const
    {
        return nameDecl_;
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
    void Struct::accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Depth depth)
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
