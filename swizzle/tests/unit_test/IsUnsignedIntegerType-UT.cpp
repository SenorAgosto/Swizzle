#include "./ut_support/UnitTestSupport.hpp"
#include <swizzle/IsUnsignedIntegerType.hpp>

namespace {

    using namespace swizzle;

    TEST(verifyIsType)
    {
        CHECK(!IsUnsignedIntegerType("case"));
        CHECK(!IsUnsignedIntegerType("const"));
        CHECK(!IsUnsignedIntegerType("enum"));
        CHECK(!IsUnsignedIntegerType("import"));
        CHECK(!IsUnsignedIntegerType("namespace"));

        CHECK(!IsUnsignedIntegerType("struct"));
        CHECK(!IsUnsignedIntegerType("using"));
        CHECK(!IsUnsignedIntegerType("bitfield"));
        CHECK(!IsUnsignedIntegerType("f32"));
        CHECK(!IsUnsignedIntegerType("f64"));
        CHECK(!IsUnsignedIntegerType("variable_block"));

        CHECK(IsUnsignedIntegerType("u8"));
        CHECK(!IsUnsignedIntegerType("i8"));
        CHECK(IsUnsignedIntegerType("u16"));
        CHECK(!IsUnsignedIntegerType("i16"));
        CHECK(IsUnsignedIntegerType("u32"));
        CHECK(!IsUnsignedIntegerType("i32"));
        CHECK(IsUnsignedIntegerType("u64"));
        CHECK(!IsUnsignedIntegerType("i64"));
    }
}
