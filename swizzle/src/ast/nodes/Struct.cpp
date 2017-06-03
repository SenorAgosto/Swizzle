#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

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

    void Struct::accept(VisitorInterface& visitor)
    {
        visitor(*this);

        for(auto& child : children())
        {
            child->accept(visitor);
        }
    }
}}}
