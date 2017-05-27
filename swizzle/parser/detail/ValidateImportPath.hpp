#pragma once

namespace boost { namespace filesystem {
    class path;
}}

namespace swizzle { namespace parser { namespace detail {

    void validateImportPath(const boost::filesystem::path& import);
}}}
