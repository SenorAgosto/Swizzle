#include <swizzle/ast/nodes/Import.hpp>
#include <swizzle/ast/AbstractTreeVertexVisitorInterface.hpp>

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
}}}
