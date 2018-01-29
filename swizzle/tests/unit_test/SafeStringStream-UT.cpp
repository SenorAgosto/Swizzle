#include "./ut_support/UnitTestSupport.hpp"
#include <swizzle/types/SafeStringStream.hpp>

#include <swizzle/Exceptions.hpp>
#include <boost/utility/string_view.hpp>

namespace {

    using namespace swizzle::lexer;
    using namespace swizzle::types;

    struct SafeStringStreamFixture
    {
        const boost::string_view sv = boost::string_view("198");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::numeric_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(SafeStringStreamFixture, verifyConstruction)
    {
    }

    struct WhenInputIsU8
    {
        std::uint8_t i = 0;

        const boost::string_view sv = boost::string_view("42");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::numeric_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(WhenInputIsU8, verifyInsertion)
    {
        ss >> i;
        CHECK_EQUAL(42U, static_cast<std::size_t>(i));
    }

    struct WhenInputIsI8
    {
        std::int8_t i = 0;

        const boost::string_view sv = boost::string_view("-128");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::numeric_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(WhenInputIsI8, verifyInsertion)
    {
        ss >> i;
        CHECK_EQUAL(-128, static_cast<std::ptrdiff_t>(i));
    }

    struct WhenInputIsHexU8
    {
        std::uint8_t i = 0;

        const boost::string_view sv = boost::string_view("0xff");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::hex_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(WhenInputIsHexU8, verifyInsertionHexModifier)
    {
        ss >> hex_stream >> i;

        CHECK(ss.hex_stream());
        CHECK_EQUAL(255U, static_cast<unsigned int>(i));
    }

    struct WhenInputIsU16
    {
        std::uint16_t i = 0;

        const boost::string_view sv = boost::string_view("16500");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::numeric_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(WhenInputIsU16, verifyInsertion)
    {
        ss >> i;
        CHECK_EQUAL(16500U, i);
    }

    struct WhenInputIsI16
    {
        std::int16_t i = 0;

        const boost::string_view sv = boost::string_view("-405");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::numeric_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(WhenInputIsI16, verifyInsertion)
    {
        ss >> i;
        CHECK_EQUAL(-405, i);
    }

    struct WhenInputIsU32
    {
        std::uint32_t i = 0;

        const boost::string_view sv = boost::string_view("65537");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::numeric_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(WhenInputIsU32, verifyInsertion)
    {
        ss >> i;
        CHECK_EQUAL(65537U, i);
    }

    struct WhenInputIsI32
    {
        std::int32_t i = 0;

        const boost::string_view sv = boost::string_view("65557");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::numeric_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(WhenInputIsI32, verifyInsertion)
    {
        ss >> i;
        CHECK_EQUAL(65557, i);
    }

    struct WhenInputIsU64
    {
        std::uint64_t i = 0;

        const boost::string_view sv = boost::string_view("54654654654");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::numeric_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(WhenInputIsU64, verifyInsertion)
    {
        ss >> i;
        CHECK_EQUAL(54654654654UL, i);
    }

    struct WhenInputIsI64
    {
        std::int64_t i = 0;

        const boost::string_view sv = boost::string_view("-54654654654");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::numeric_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(WhenInputIsI64, verifyInsertion)
    {
        ss >> i;
        CHECK_EQUAL(-54654654654L, i);
    }

    struct WhenInputIsEmpty
    {
        const boost::string_view sv;
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::numeric_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    struct WhenInputIsEmptyAndReadingU8 : public WhenInputIsEmpty
    {
        std::uint8_t i = 0;
    };

    TEST_FIXTURE(WhenInputIsEmptyAndReadingU8, verifyInsertionThrows)
    {
        CHECK_THROW(ss >> i, swizzle::StreamEmpty);
    }

    struct WhenInputIsEmptyAndReadingI8 : public WhenInputIsEmpty
    {
        std::int8_t i = 0;
    };

    TEST_FIXTURE(WhenInputIsEmptyAndReadingI8, verifyInsertionThrows)
    {
        CHECK_THROW(ss >> i, swizzle::StreamEmpty);
    }

    struct WhenInputIsEmptyAndReadingU16 : public WhenInputIsEmpty
    {
        std::uint16_t i = 0;
    };

    TEST_FIXTURE(WhenInputIsEmptyAndReadingU16, verifyInsertionThrows)
    {
        CHECK_THROW(ss >> i, swizzle::StreamEmpty);
    }

    struct WhenInputIsEmptyAndReadingI16 : public WhenInputIsEmpty
    {
        std::int16_t i = 0;
    };

    TEST_FIXTURE(WhenInputIsEmptyAndReadingI16, verifyInsertionThrows)
    {
        CHECK_THROW(ss >> i, swizzle::StreamEmpty);
    }

    struct WhenInputIsEmptyAndReadingU32 : public WhenInputIsEmpty
    {
        std::uint32_t i = 0;
    };

    TEST_FIXTURE(WhenInputIsEmptyAndReadingU32, verifyInsertionThrows)
    {
        CHECK_THROW(ss >> i, swizzle::StreamEmpty);
    }

    struct WhenInputIsEmptyAndReadingI32 : public WhenInputIsEmpty
    {
        std::int32_t i = 0;
    };

    TEST_FIXTURE(WhenInputIsEmptyAndReadingI32, verifyInsertionThrows)
    {
        CHECK_THROW(ss >> i, swizzle::StreamEmpty);
    }

    struct WhenInputIsEmptyAndReadingU64 : public WhenInputIsEmpty
    {
        std::uint64_t i = 0;
    };

    TEST_FIXTURE(WhenInputIsEmptyAndReadingU64, verifyInsertionThrows)
    {
        CHECK_THROW(ss >> i, swizzle::StreamEmpty);
    }

    struct WhenInputIsEmptyAndReadingI64 : public WhenInputIsEmpty
    {
        std::int64_t i = 0;
    };

    TEST_FIXTURE(WhenInputIsEmptyAndReadingI64, verifyInsertionThrows)
    {
        CHECK_THROW(ss >> i, swizzle::StreamEmpty);
    }

    struct WhenInputIsInvalidHexU8
    {
        std::uint8_t i = 0;

        const boost::string_view sv = boost::string_view("1234");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::numeric_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(WhenInputIsInvalidHexU8, verifyInsertionHexModifierThrows)
    {
        CHECK_THROW(ss >> hex_stream >> i, swizzle::InvalidStreamInput);
    }

    struct WhenInputIsInvalidHexU8_2
    {
        std::uint8_t i = 0;

        const boost::string_view sv = boost::string_view("0xAZ");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::hex_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(WhenInputIsInvalidHexU8_2, verifyInsertionHexModifierThrows)
    {
        CHECK_THROW(ss >> hex_stream >> i, swizzle::InvalidStreamInput);
    }

    struct WhenInputIsU8AndOverflows
    {
        std::uint8_t i = 0;

        const boost::string_view sv = boost::string_view("300");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::numeric_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(WhenInputIsU8AndOverflows, verifyInsertionThrows)
    {
        CHECK_THROW(ss >> i, swizzle::StreamInputCausesOverflow);
    }

    struct WhenInputIsI8AndOverflows
    {
        std::int8_t i = 0;

        const boost::string_view sv = boost::string_view("130");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::numeric_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(WhenInputIsI8AndOverflows, verifyInsertionThrows)
    {
        CHECK_THROW(ss >> i, swizzle::StreamInputCausesOverflow);
    }

    struct WhenInputIsI8AndUnderflows
    {
        std::int8_t i = 0;

        const boost::string_view sv = boost::string_view("-130");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::numeric_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(WhenInputIsI8AndUnderflows, verifyInsertionThrows)
    {
        CHECK_THROW(ss >> i, swizzle::StreamInputCausesUnderflow);
    }

    struct WhenInputIsU16AndInputOverflows
    {
        std::uint16_t i = 0;

        const boost::string_view sv = boost::string_view("65536");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::numeric_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(WhenInputIsU16AndInputOverflows, verifyInsertionThrows)
    {
        CHECK_THROW(ss >> i, swizzle::StreamInputCausesOverflow);
    }

    struct WhenInputIsI16AndInputOverflows
    {
        std::int16_t i = 0;

        const boost::string_view sv = boost::string_view("32800");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::numeric_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(WhenInputIsI16AndInputOverflows, verifyInsertionThrows)
    {
        CHECK_THROW(ss >> i, swizzle::StreamInputCausesOverflow);
    }

    struct WhenInputIsI16AndInputUnderflows
    {
        std::int16_t i = 0;

        const boost::string_view sv = boost::string_view("-32800");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::numeric_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(WhenInputIsI16AndInputUnderflows, verifyInsertionThrows)
    {
        CHECK_THROW(ss >> i, swizzle::StreamInputCausesUnderflow);
    }

    struct WhenInputIsU32AndInputOverflows
    {
        std::uint32_t i = 0;

        const boost::string_view sv = boost::string_view("4294967296");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::numeric_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(WhenInputIsU32AndInputOverflows, verifyInsertionThrows)
    {
        CHECK_THROW(ss >> i, swizzle::StreamInputCausesOverflow);
    }

    struct WhenInputIsI32AndInputOverflows
    {
        std::int32_t i = 0;

        const boost::string_view sv = boost::string_view("2147483648");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::numeric_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(WhenInputIsI32AndInputOverflows, verifyInsertionThrows)
    {
        CHECK_THROW(ss >> i, swizzle::StreamInputCausesOverflow);
    }

    struct WhenInputIsI32AndInputUnderflows
    {
        std::int32_t i = 0;

        const boost::string_view sv = boost::string_view("-2147483650");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::numeric_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(WhenInputIsI32AndInputUnderflows, verifyInsertionThrows)
    {
        CHECK_THROW(ss >> i, swizzle::StreamInputCausesUnderflow);
    }

    struct WhenInputIsU64AndInputOverflows
    {
        std::uint64_t i = 0;

        const boost::string_view sv = boost::string_view("184467440737095500000000000000000000");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::numeric_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(WhenInputIsU64AndInputOverflows, verifyInsertionThrows)
    {
        CHECK_THROW(ss >> i, swizzle::StreamInputCausesOverflow);
    }


    struct WhenInputIsI64AndInputOverflows
    {
        std::int64_t i = 0;

        const boost::string_view sv = boost::string_view("9223372036854775808");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::numeric_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(WhenInputIsI64AndInputOverflows, verifyInsertionThrows)
    {
        CHECK_THROW(ss >> i, swizzle::StreamInputCausesOverflow);
    }

    struct WhenInputIsI64AndInputUnderflows
    {
        std::int64_t i = 0;

        const boost::string_view sv = boost::string_view("-9223372036854775809");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::numeric_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(WhenInputIsI64AndInputUnderflows, verifyInsertionThrows)
    {
        CHECK_THROW(ss >> i, swizzle::StreamInputCausesUnderflow);
    }

    struct WhenInputIsI64AndInputVastlyOverflows
    {
        std::int64_t i = 0;

        const boost::string_view sv = boost::string_view("-922337203685477580900000000000000001");
        const TokenInfo token = TokenInfo(Token(sv, 0, sv.length(), TokenType::numeric_literal), FileInfo("test.swizzle"));
        
        safe_istringstream ss = safe_istringstream(token);
    };

    TEST_FIXTURE(WhenInputIsI64AndInputVastlyOverflows, verifyInsertionThrows)
    {
        CHECK_THROW(ss >> i, std::runtime_error);
    }

}
