#include <swizzle/ast/nodes/TypeAlias.hpp>
#include <swizzle/ast/AbstractTreeVertexVisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    TypeAlias::TypeAlias(const lexer::TokenInfo& info, const lexer::TokenInfo& aliasedInfo)
        : info_(info)
        , aliasedType_(aliasedInfo)
        , existingType_(lexer::Token(), lexer::FileInfo(info.fileInfo().filename()))
    {
    }

    const lexer::TokenInfo& TypeAlias::info() const
    {
        return info_;
    }

    const lexer::TokenInfo& TypeAlias::aliasedType() const
    {
        return aliasedType_;
    }

    void TypeAlias::existingType(const lexer::TokenInfo& info)
    {
        existingType_ = info;
    }

    const lexer::TokenInfo& TypeAlias::existingType() const
    {
        return existingType_;
    }

}}}
