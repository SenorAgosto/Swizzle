#include <swizzle/ast/nodes/Enum.hpp>

#include <swizzle/ast/AncestorInfo.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    Enum::Enum(const lexer::TokenInfo& enumInfo, const lexer::TokenInfo& name, const std::string& containingNamespace)
        : enumInfo_(enumInfo)
        , nameInfo_(name)
        , name_(containingNamespace + "::" + nameInfo_.token().to_string())
    {
    }

    const lexer::TokenInfo& Enum::enumInfo() const
    {
        return enumInfo_;
    }

    const lexer::TokenInfo& Enum::nameInfo() const
    {
        return nameInfo_;
    }

    std::string Enum::name() const
    {
        return name_;
    }

    void Enum::underlying(const lexer::TokenInfo& value)
    {
        underlyingType_ = value;
    }

    const lexer::TokenInfo& Enum::underlying() const
    {
        return underlyingType_;
    }

    void Enum::accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Node::Depth depth)
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
