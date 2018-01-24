#include <swizzle/ast/nodes/Import.hpp>

#include <swizzle/ast/AncestorInfo.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    Import::Import(const lexer::TokenInfo& info, const boost::filesystem::path& path)
        : info_(info)
        , importPath_(path)
    {
    }

    const lexer::TokenInfo& Import::info() const
    {
        return info_;
    }

    const boost::filesystem::path& Import::path() const
    {
        return importPath_;
    }

    void Import::accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Node::Depth depth)
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
