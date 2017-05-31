#include "./platform/UnitTestSupport.hpp"
#include <swizzle/IsType.hpp>

namespace {

    using namespace swizzle;

    TEST(verifyIsType)
    {
        CHECK(!IsType("case"));
        CHECK(!IsType("const"));
        CHECK(!IsType("enum"));
        CHECK(!IsType("import"));
        CHECK(!IsType("namespace"));

        CHECK(!IsType("struct"));
        CHECK(!IsType("using"));

        CHECK(IsType("bitfield"));
        CHECK(IsType("u8"));
        CHECK(IsType("i8"));
        CHECK(IsType("u16"));
        CHECK(IsType("i16"));
        CHECK(IsType("u32"));
        CHECK(IsType("i32"));
        CHECK(IsType("u64"));
        CHECK(IsType("i64"));
        CHECK(IsType("f32"));
        CHECK(IsType("f64"));
        CHECK(IsType("variable_block"));
    }
}
