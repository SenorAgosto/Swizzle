#include <swizzle/ast/nodes/Enum.hpp>
#include <swizzle/ast/AbstractTreeVertexVisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    Enum::Enum(const lexer::TokenInfo& info, const std::string& containingNamespace)
        : info_(info)
        , name_(containingNamespace + "::" + info_.token().to_string())
    {
    }

    const lexer::TokenInfo& Enum::info() const
    {
        return info_;
    }

    std::string Enum::name() const
    {
        return name_;
    }
}}}
