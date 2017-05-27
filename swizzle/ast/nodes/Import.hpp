#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

#include <boost/filesystem/path.hpp>

namespace swizzle { namespace ast { namespace nodes {

    class Import : public Node
    {
    public:
        Import(const lexer::TokenInfo& info, const boost::filesystem::path& path);

        const lexer::TokenInfo& info() const;
        const boost::filesystem::path& path() const;
        
    private:
        lexer::TokenInfo info_;
        boost::filesystem::path importPath_;
    };
}}}
