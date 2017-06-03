#include "./platform/UnitTestSupport.hpp"
#include <swizzle/IsIntegerType.hpp>

namespace {

    using namespace swizzle;

    TEST(verifyIsType)
    {
        CHECK(!IsIntegerType("case"));
        CHECK(!IsIntegerType("const"));
        CHECK(!IsIntegerType("enum"));
        CHECK(!IsIntegerType("import"));
        CHECK(!IsIntegerType("namespace"));

        CHECK(!IsIntegerType("struct"));
        CHECK(!IsIntegerType("using"));
        CHECK(!IsIntegerType("bitfield"));
        CHECK(!IsIntegerType("f32"));
        CHECK(!IsIntegerType("f64"));
        CHECK(!IsIntegerType("variable_block"));

        CHECK(IsIntegerType("u8"));
        CHECK(IsIntegerType("i8"));
        CHECK(IsIntegerType("u16"));
        CHECK(IsIntegerType("i16"));
        CHECK(IsIntegerType("u32"));
        CHECK(IsIntegerType("i32"));
        CHECK(IsIntegerType("u64"));
        CHECK(IsIntegerType("i64"));
    }
}
