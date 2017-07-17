#include "./ut_support/UnitTestSupport.hpp"

#include <swizzle/types/ReadAs.hpp>
#include <boost/utility/string_view.hpp>

namespace {

    using namespace swizzle::types;

    static const bool isHex = true;
    static const bool isNotHex = false;

    TEST(verifyReadHexToU8)
    {
        boost::string_view sv("0xff");
        std::uint8_t i = readAsU8<isHex>(sv);

        CHECK_EQUAL(255U, i);
    }

    TEST(verifyReadToU8)
    {
        boost::string_view sv("125");
        std::uint8_t i = readAsU8<isNotHex>(sv);

        CHECK_EQUAL(125U, i);

    }

    TEST(verifyReadHexToI8)
    {
        boost::string_view sv("0x0f");
        std::int8_t i = readAsU8<isHex>(sv);

        CHECK_EQUAL(15, i);
    }

    TEST(verifyReadToI8)
    {
        boost::string_view sv("125");
        std::int8_t i = readAsU8<isNotHex>(sv);

        CHECK_EQUAL(125, i);

    }

    TEST(verifyReadHexToU16)
    {
        boost::string_view sv("0xffff");
        std::uint16_t i = readAs<std::uint16_t, isHex>(sv);

        CHECK_EQUAL(65535U, i);
    }

    TEST(verifyReadToU16)
    {
        boost::string_view sv("161");
        std::uint16_t i = readAs<std::uint16_t, isNotHex>(sv);

        CHECK_EQUAL(161U, i);
    }

    TEST(verifyReadHexToI16)
    {
        boost::string_view sv("0x00ff");
        std::int16_t i = readAs<std::int16_t, isHex>(sv);

        CHECK_EQUAL(255, i);
    }

    TEST(verifyReadToI16)
    {
        boost::string_view sv("161");
        std::int16_t i = readAs<std::int16_t, isNotHex>(sv);

        CHECK_EQUAL(161, i);
    }

    TEST(verifyReadHexToU32)
    {
        boost::string_view sv("0xffff");
        std::uint32_t i = readAs<std::uint32_t, isHex>(sv);

        CHECK_EQUAL(65535U, i);
    }

    TEST(verifyReadToU32)
    {
        boost::string_view sv("161");
        std::uint32_t i = readAs<std::uint32_t, isNotHex>(sv);

        CHECK_EQUAL(161U, i);
    }

    TEST(verifyReadHexToI32)
    {
        boost::string_view sv("0x00ff");
        std::int32_t i = readAs<std::int32_t, isHex>(sv);

        CHECK_EQUAL(255, i);
    }

    TEST(verifyReadToI32)
    {
        boost::string_view sv("161");
        std::int32_t i = readAs<std::int32_t, isNotHex>(sv);

        CHECK_EQUAL(161, i);
    }


    TEST(verifyReadHexToU64)
    {
        boost::string_view sv("0x0000ffff");
        std::uint64_t i = readAs<std::uint64_t, isHex>(sv);

        CHECK_EQUAL(65535U, i);
    }

    TEST(verifyReadToU64)
    {
        boost::string_view sv("161");
        std::uint64_t i = readAs<std::uint64_t, isNotHex>(sv);

        CHECK_EQUAL(161U, i);
    }

    TEST(verifyReadHexToI64)
    {
        boost::string_view sv("0x00ff");
        std::int64_t i = readAs<std::int64_t, isHex>(sv);

        CHECK_EQUAL(255, i);
    }

    TEST(verifyReadToI64)
    {
        boost::string_view sv("161");
        std::int64_t i = readAs<std::int64_t, isNotHex>(sv);

        CHECK_EQUAL(161, i);
    }
}
