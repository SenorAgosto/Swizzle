#include "./platform/UnitTestSupport.hpp"
#include <swizzle/lexer/LineInfo.hpp>

namespace {

    using namespace swizzle::lexer;
    
    struct LineInfoFixture
    {
        LineInfoFixture()
            : li2(100, 20)
        {
        }
        
        LineInfo li;
        LineInfo li2;
    };
    
    TEST_FIXTURE(LineInfoFixture, verifyInstantation)
    {
        CHECK_EQUAL(1U, li.line());
        CHECK_EQUAL(1U, li.column());
        
        CHECK_EQUAL(100U, li2.line());
        CHECK_EQUAL(20U, li2.column());
    }
    
    TEST_FIXTURE(LineInfoFixture, verifyIncrementColumn)
    {
        CHECK_EQUAL(1U, li.column());
        
        li.incrementColumn();
        CHECK_EQUAL(2U, li.column());
    }

    TEST_FIXTURE(LineInfoFixture, verifyIncrementColumnBy)
    {
        CHECK_EQUAL(1U, li.column());
        
        li.incrementColumnBy(20);
        CHECK_EQUAL(21U, li.column());
    }
    
    TEST_FIXTURE(LineInfoFixture, verifyIncrementLine)
    {
        CHECK_EQUAL(1U, li.line());
        
        li.incrementLine();
        CHECK_EQUAL(2U, li.line());
    }
    
    TEST_FIXTURE(LineInfoFixture, verifyOStream)
    {
        std::stringstream ss;
        ss << li;
        
        CHECK_EQUAL("1:1", ss.str());
        
        std::stringstream ss2;
        ss2 << li2;
        
        CHECK_EQUAL("100:20", ss2.str());
    }
    
    TEST_FIXTURE(LineInfoFixture, verifyLessThan)
    {
        CHECK(li < li2);
    }
    
    TEST_FIXTURE(LineInfoFixture, verifyEqual)
    {
        LineInfo li3;
        CHECK(li == li3);
    }
    
    TEST_FIXTURE(LineInfoFixture, verifyGreaterThan)
    {
        CHECK(li2 > li);
    }
}
