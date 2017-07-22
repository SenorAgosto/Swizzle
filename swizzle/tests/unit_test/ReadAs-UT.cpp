#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/Exceptions.hpp>
#include <swizzle/types/ReadAs.hpp>

#include <boost/numeric/conversion/cast.hpp>
#include <boost/utility/string_view.hpp>

namespace {

    using namespace swizzle::types;

    static const bool isHex = true;
    static const bool isNotHex = false;

    struct GivenHexU8
    {
        const boost::string_view sv = boost::string_view("0xff");
    };

    TEST_FIXTURE(GivenHexU8, verifyReadAsHexU8)
    {
        std::uint8_t i = readAsHexU8(sv);
        CHECK_EQUAL(255U, i);
    }

    struct GivenU8
    {
        const boost::string_view sv = boost::string_view("125");
    };

    TEST_FIXTURE(GivenU8, verifyReadAsU8)
    {
        std::uint8_t i = readAsU8(sv);
        CHECK_EQUAL(125U, i);
    }

    struct GivenHexI8
    {
        const boost::string_view sv = boost::string_view("0x0f");
    };

    TEST_FIXTURE(GivenHexI8, verifyReadAsHexI8)
    {
        std::int8_t i = readAsHexI8(sv);
        CHECK_EQUAL(15, i);
    }

    struct GivenI8
    {
        const boost::string_view sv = boost::string_view("-125");
    };

    TEST_FIXTURE(GivenI8, verifyReadAsI8)
    {
        std::int8_t i = readAsI8(sv);
        CHECK_EQUAL(-125, i);
    }

    struct GivenHexU16
    {
        const boost::string_view sv = boost::string_view("0xffff");
    };

    TEST_FIXTURE(GivenHexU16, verifyReadAsHex)
    {
        std::uint16_t i = readAsHex<std::uint16_t>(sv);
        CHECK_EQUAL(65535U, i);
    }

    struct GivenU16
    {
        const boost::string_view sv = boost::string_view("161");
    };

    TEST_FIXTURE(GivenU16, verifyReadAs)
    {
        std::uint16_t i = readAs<std::uint16_t>(sv);
        CHECK_EQUAL(161U, i);
    }

    struct GivenHexI16
    {
        const boost::string_view sv = boost::string_view("0x00ff");
    };

    TEST_FIXTURE(GivenHexI16, verifyReadAsHex)
    {
        std::int16_t i = readAsHex<std::int16_t>(sv);
        CHECK_EQUAL(255, i);
    }

    struct GivenI16
    {
        const boost::string_view sv = boost::string_view("161");
    };

    TEST_FIXTURE(GivenI16, verifyReadAs)
    {
        std::int16_t i = readAs<std::int16_t>(sv);
        CHECK_EQUAL(161, i);
    }

    struct GivenHexU32
    {
        const boost::string_view sv = boost::string_view("0x0000ffff");
    };

    TEST_FIXTURE(GivenHexU32, verifyReadAsHex)
    {
        std::uint32_t i = readAsHex<std::uint32_t>(sv);
        CHECK_EQUAL(65535U, i);
    }

    struct GivenU32
    {
        const boost::string_view sv = boost::string_view("161");
    };

    TEST_FIXTURE(GivenU32, verifyReadAs)
    {
        std::uint32_t i = readAs<std::uint32_t>(sv);
        CHECK_EQUAL(161U, i);
    }

    struct GivenHexI32
    {
        const boost::string_view sv = boost::string_view("0x800000ff");
    };

    TEST_FIXTURE(GivenHexI32, verifyReadAsHex)
    {
        std::int32_t i = readAsHex<std::int32_t>(sv);
        CHECK_EQUAL(-255, i);
    }

    struct GivenI32
    {
        const boost::string_view sv = boost::string_view("161");
    };

    TEST_FIXTURE(GivenI32, verifyReadAs)
    {
        std::int32_t i = readAs<std::int32_t>(sv);
        CHECK_EQUAL(161, i);
    }

    struct GivenHexU64
    {
        const boost::string_view sv = boost::string_view("0x000000000000ffff");
    };

    TEST_FIXTURE(GivenHexU64, verifyReadAsHex)
    {
        std::uint64_t i = readAsHex<std::uint64_t>(sv);
        CHECK_EQUAL(65535U, i);
    }

    struct GivenU64
    {
        const boost::string_view sv = boost::string_view("161");
    };

    TEST_FIXTURE(GivenU64, verifyReadAs)
    {
        std::uint64_t i = readAs<std::uint64_t>(sv);
        CHECK_EQUAL(161U, i);
    }

    struct GivenHexI64
    {
        const boost::string_view sv = boost::string_view("0x00000000000000ff");
    };

    TEST_FIXTURE(GivenHexI64, verifyReadAsHex)
    {
        std::int64_t i = readAsHex<std::int64_t>(sv);
        CHECK_EQUAL(255, i);
    }

    struct GivenI64
    {
        const boost::string_view sv = boost::string_view("161 ", 3);
    };

    TEST_FIXTURE(GivenI64, verifyReadAs)
    {
        std::int64_t i = readAs<std::int64_t>(sv);
        CHECK_EQUAL(161, i);
    }

    struct GivenTooLargeHexU8
    {
        const boost::string_view sv = boost::string_view("0xfff");
    };

    TEST_FIXTURE(GivenTooLargeHexU8, verifyReadAsHexU8Throws)
    {
        CHECK_THROW(std::uint8_t i = readAsHexU8(sv), std::out_of_range);
    }

    struct GivenTooLargeU8
    {
        const boost::string_view sv = boost::string_view("300");
    };

    TEST_FIXTURE(GivenTooLargeU8, verifyReadAsU8Throws)
    {
        CHECK_THROW(std::uint8_t i = readAsU8(sv), boost::bad_numeric_cast);
    }

    struct GivenTooLargeHexI8
    {
        const boost::string_view sv = boost::string_view("0x0ff");
    };

    TEST_FIXTURE(GivenTooLargeHexI8, verifyReadAsHexI8Throws)
    {
        CHECK_THROW(std::int8_t i = readAsHexI8(sv), std::out_of_range);
    }

    struct GivenTooLargeI8
    {
        const boost::string_view sv = boost::string_view("130");
    };

    TEST_FIXTURE(GivenTooLargeI8, verifyReadAsI8Throws)
    {
        CHECK_THROW(std::int8_t i = readAsI8(sv), boost::bad_numeric_cast);
    }

    struct GivenTooLargeHexU16
    {
        const boost::string_view sv = boost::string_view("0xffffff");
    };

    TEST_FIXTURE(GivenTooLargeHexU16, verifyReadAsHexThrows)
    {
        CHECK_THROW(std::uint16_t i = readAsHex<std::uint16_t>(sv), std::out_of_range);
    }

    struct GivenTooLargeU16
    {
        const boost::string_view sv = boost::string_view("65537");
    };

    TEST_FIXTURE(GivenTooLargeU16, verifyReadAsThrows)
    {
        CHECK_THROW(std::uint16_t i = readAs<std::uint16_t>(sv), boost::bad_numeric_cast);
    }

    struct GivenTooLargeHexU32
    {
        const boost::string_view sv = boost::string_view("0xffffffffffff");
    };

    TEST_FIXTURE(GivenTooLargeHexU32, verifyReadAsHexThrows)
    {
        CHECK_THROW(std::uint32_t i = readAsHex<std::uint32_t>(sv), std::out_of_range);
    }

    struct GivenTooLargeU32
    {
        const boost::string_view sv = boost::string_view("8589934592");
    };

    TEST_FIXTURE(GivenTooLargeU32, verifyReadAsThrows)
    {
        CHECK_THROW(std::uint32_t i = readAs<std::uint32_t>(sv), boost::bad_numeric_cast);
    }

    struct GivenTooLargeHexU64
    {
        const boost::string_view sv = boost::string_view("0xffffffffffff");
    };

    TEST_FIXTURE(GivenTooLargeHexU64, verifyReadAsHexThrows)
    {
        CHECK_THROW(std::uint64_t i = readAsHex<std::uint64_t>(sv), std::out_of_range);
    }

    struct GivenTooLargeU64
    {
        const boost::string_view sv = boost::string_view("18446744073709550000000000000000000000");
    };

    TEST_FIXTURE(GivenTooLargeU64, verifyReadAsThrows)
    {
        CHECK_THROW(std::uint32_t i = readAs<std::uint32_t>(sv), boost::bad_numeric_cast);
    }
}
