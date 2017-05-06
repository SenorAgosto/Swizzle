#pragma once 
#include <swizzle/lexer/LineInfo.hpp>

#include <cstddef>
#include <string>

namespace swizzle { namespace lexer {
    class Token;
}}

namespace swizzle { namespace lexer {

    // Information about where a token starts and ends
    class FileInfo
    {
    public:
        FileInfo(const std::string& filename);
        FileInfo(const std::string& filename, const LineInfo& start);
        FileInfo(const std::string& filename, const LineInfo& start, const LineInfo& end);
            
        const std::string& filename() const { return filename_; }
    
        const LineInfo& end() const { return end_; }
        LineInfo& end() { return end_; }
        
        const LineInfo& start() const { return start_; }
        LineInfo& start() { return start_; }
        
        void incrementLine() { end_.incrementLine(); }
        void incrementColumn() { end_.incrementColumn(); }
        void incrementColumnBy(const std::size_t count) { end_.incrementColumnBy(count); }
        void advanceBy(const Token& token);
        
        bool empty() const { return start_ == end_; }
        
    private:
        std::string filename_;
        
        LineInfo start_;
        LineInfo end_;
    };
}}
