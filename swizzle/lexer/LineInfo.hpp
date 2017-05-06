#pragma once 
#include <cstddef>
#include <ostream>

namespace swizzle { namespace lexer {

    class LineInfo
    {
    public:
        LineInfo();
        LineInfo(const std::size_t line, const std::size_t column);
        
        std::size_t line() const { return line_; }
        std::size_t column() const { return column_; }
        
        void incrementColumn() { column_++; }
        void incrementColumnBy(const std::size_t count) { column_ += count; }
        void incrementLine() { line_++; column_ = 1; }
        
    private:
        std::size_t line_;
        std::size_t column_;
    };
    
    std::ostream& operator<<(std::ostream& os, const LineInfo& info);
    
    bool operator<(const LineInfo& lhs, const LineInfo& rhs);
    bool operator==(const LineInfo& lhs, const LineInfo& rhs);
    bool operator>(const LineInfo& lhs, const LineInfo& rhs);
}}
