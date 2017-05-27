#include <swizzle/parser/detail/ValidateImportPath.hpp>
#include <swizzle/Exceptions.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace boosct { namespace filesystem {
    class path;
}}

namespace swizzle { namespace parser { namespace detail {

    void validateImportPath(const boost::filesystem::path& import)
    {
        if(boost::filesystem::is_directory(import))
        {
            throw ParserError("Import file is directory: " + import.string());
        }

        if(!boost::filesystem::exists(import))
        {
            throw ParserError("Import file doesn't exist: " + import.string());
        }
    }

}}}
