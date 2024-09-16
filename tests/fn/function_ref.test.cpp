#include <cpputil/function_ref.hpp>
#include <cppunittest/unittest.hpp>

static_assert(!std::is_default_constructible_v<cpputil::fn::Function_ref<void(int)>>);
static_assert(std::is_trivially_copyable_v<cpputil::fn::Function_ref<void(int)>>);
static_assert(std::is_trivially_destructible_v<cpputil::fn::Function_ref<void(int)>>);

namespace {
    auto mul(int a, int b) -> int
    {
        return a * b;
    }

    auto add(int a, int b) -> int
    {
        return a + b;
    }
} // namespace

UNITTEST("function_ref pointer")
{
    cpputil::fn::Function_ref<int(int, int)> ref = mul;
    REQUIRE_EQUAL(ref(10, 20), 200);
    ref = add;
    REQUIRE_EQUAL(ref(10, 20), 30);
}

UNITTEST("function_ref object")
{
    int out {};

    auto const increment = [&out](int x) { out += x; };
    auto const ref       = cpputil::fn::Function_ref<void(int) const>(increment);

    REQUIRE_EQUAL(out, 0);
    ref(5);
    REQUIRE_EQUAL(out, 5);
    ref(10);
    REQUIRE_EQUAL(out, 15);
}

UNITTEST("function_ref assignment")
{
    int out {};

    cpputil::fn::Function_ref<int(int, int) const> ref = mul;

    REQUIRE_EQUAL(out, 0);
    REQUIRE_EQUAL(ref(2, 3), 6);

    auto const f = [&out](int a, int b) {
        out = a - b;
        return 0;
    };
    ref = f;

    REQUIRE_EQUAL(ref(3, 1), 0);
    REQUIRE_EQUAL(out, 2);
    REQUIRE_EQUAL(ref(20, 3), 0);
    REQUIRE_EQUAL(out, 17);

    ref = add;

    REQUIRE_EQUAL(ref(10, 15), 25);
    REQUIRE_EQUAL(ref(3, 5), 8);
}
