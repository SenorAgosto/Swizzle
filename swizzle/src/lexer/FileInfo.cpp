#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/Token.hpp>

#include <utility>

namespace swizzle { namespace lexer {

    FileInfo::FileInfo(const std::string& filename)
        : filename_(filename)
    {
    }
    
    FileInfo::FileInfo(const std::string& filename, const LineInfo& start)
        : filename_(filename)
        , start_(start)
        , end_(start)
    {
    }
    
    FileInfo::FileInfo(const std::string& filename, const LineInfo& start, const LineInfo& end)
        : filename_(filename)
        , start_(start)
        , end_(end)
    {
    }

    void FileInfo::advanceBy(const char c)
    {
        if(c == '\n')
        {
            end_.incrementLine();
        }
        else
        {
            end_.incrementColumn();
        }
    }

    void FileInfo::advanceBy(const Token& token)
    {
        for(auto c : token.value())
        {
            advanceBy(c);
        }
    }

    void FileInfo::advanceTo(const FileInfo& info)
    {
        start_ = info.end_;

        end_ = start_;
        end_.incrementColumn();
    }
}}
