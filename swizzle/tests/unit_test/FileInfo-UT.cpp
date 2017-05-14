#include "./platform/UnitTestSupport.hpp"

#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/LineInfo.hpp>
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenType.hpp>

namespace {

    using namespace swizzle::lexer;
    
    struct FileInfoFixture
    {
        FileInfoFixture()
            : start(100, 42)
            , end(100, 50)
            , info("test.dot", start, end)
        {
        }
        
        LineInfo start;
        LineInfo end;
        FileInfo info;
    };

    TEST(verifyConstructionWithFilenameOnly)
    {
        FileInfo info("test.dot");
        LineInfo line;
        
        CHECK_EQUAL("test.dot", info.filename());
        CHECK_EQUAL(line, info.start());
        CHECK_EQUAL(line, info.end());
    }
    
    TEST(verifyConstructionWithFilenameAndStart)
    {
        LineInfo line(100, 42);
        FileInfo info("test.dot", line);
        
        CHECK_EQUAL("test.dot", info.filename());
        CHECK_EQUAL(line, info.start());
        CHECK_EQUAL(line, info.end());
    }
    
    TEST(verifyConstructionWithAllParams)
    {
        LineInfo start(100, 42);
        LineInfo end(100, 50);
        FileInfo info("test.dot", start, end);
        
        CHECK_EQUAL("test.dot", info.filename());
        CHECK_EQUAL(start, info.start());
        CHECK_EQUAL(end, info.end());
    }
    
    TEST(verifyCopyConstruction)
    {
        LineInfo start(100, 42);
        LineInfo end(100, 50);
        FileInfo info("test.dot", start, end);
        
        CHECK_EQUAL("test.dot", info.filename());
        CHECK_EQUAL(start, info.start());
        CHECK_EQUAL(end, info.end());

        FileInfo info2(info);

        CHECK_EQUAL("test.dot", info2.filename());
        CHECK_EQUAL(start, info2.start());
        CHECK_EQUAL(end, info2.end());
    }
    
    TEST(verifyMoveConstruction)
    {
        LineInfo start(100, 42);
        LineInfo end(100, 50);
        FileInfo info(FileInfo("test.dot", start, end));
        
        CHECK_EQUAL("test.dot", info.filename());
        CHECK_EQUAL(start, info.start());
        CHECK_EQUAL(end, info.end());
    }
    
    TEST_FIXTURE(FileInfoFixture, verifyIncrementLine)
    {
        CHECK_EQUAL(end, info.end());
        
        info.incrementLine();
        CHECK_EQUAL(end.line() + 1, info.end().line());
        CHECK_EQUAL(1U, info.end().column());
    }
    
    TEST_FIXTURE(FileInfoFixture, verifyIncrementColumn)
    {
        CHECK_EQUAL(end, info.end());
        
        info.incrementColumn();
        CHECK_EQUAL(end.line(), info.end().line());
        CHECK_EQUAL(end.column() + 1, info.end().column());
    }
    
    TEST_FIXTURE(FileInfoFixture, verifyIncrementColumnByValue)
    {
        CHECK_EQUAL(end, info.end());
        
        info.incrementColumnBy(4);
        CHECK_EQUAL(end.line(), info.end().line());
        CHECK_EQUAL(end.column() + 4, info.end().column());
    }
    
    TEST_FIXTURE(FileInfoFixture, verifyAdvanceBy)
    {
        Token token("variable20", 0, 10, TokenType::string);
        
        CHECK_EQUAL(end, info.end());
        
        info.advanceBy(token);
        CHECK_EQUAL(end.line(), info.end().line());
        CHECK_EQUAL(end.column() + 10, info.end().column());
    }
    
    TEST_FIXTURE(FileInfoFixture, verifyAdvanceByWithEmbededNewLines)
    {
        Token token("variable20\nvariable21\nvariable22", 0, 34, TokenType::multiline_comment);
        
        CHECK_EQUAL(end, info.end());
        
        info.advanceBy(token);
        CHECK_EQUAL(end.line() + 2, info.end().line());
        CHECK_EQUAL(11U, info.end().column());
    }

    TEST_FIXTURE(FileInfoFixture, verifyAdvanceByChar)
    {
        CHECK_EQUAL(end, info.end());

        info.advanceBy('a');
        CHECK_EQUAL(end.line(), info.end().line());
        CHECK_EQUAL(end.column() + 1, info.end().column());

        info.advanceBy('\n');
        CHECK_EQUAL(end.line() + 1, info.end().line());
        CHECK_EQUAL(1U, info.end().column());
    }

    TEST_FIXTURE(FileInfoFixture, verifyAdvanceTo)
    {
        FileInfo next(info.filename());

        CHECK_EQUAL(1U, next.start().line());
        CHECK_EQUAL(1U, next.start().column());
        CHECK_EQUAL(1U, next.end().line());
        CHECK_EQUAL(1U, next.end().column());

        next.advanceTo(info);

        CHECK_EQUAL(info.end().line(), next.start().line());
        CHECK_EQUAL(info.end().column(), next.start().column());
        CHECK_EQUAL(info.end().line(), next.end().line());
        CHECK_EQUAL(info.end().column() + 1, next.end().column());
    }

    TEST_FIXTURE(FileInfoFixture, verifyEmpty)
    {
        LineInfo empty;
        FileInfo info2("test.dot", empty, empty);
        
        CHECK(!info.empty());
        CHECK(info2.empty());
    }
}
