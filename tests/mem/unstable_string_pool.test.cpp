#include <cpputil/mem/unstable_string_pool.hpp>
#include <cppunittest/unittest.hpp>

UNITTEST("unstable string pool")
{
    cpputil::mem::Unstable_string_pool pool;

    auto const a = pool.add("hello");
    auto const b = pool.add("world");
    auto const c = pool.add("helloworld");

    CHECK_EQUAL(pool.view(a), "hello");
    CHECK_EQUAL(pool.view(b), "world");
    CHECK_EQUAL(pool.view(c), "helloworld");

    CHECK_EQUAL(a.offset, 0);
    CHECK_EQUAL(b.offset, 5);
    CHECK_EQUAL(c.offset, 0);

    CHECK(pool.add("xyz") == pool.add("xyz"));
    CHECK(pool.add("abc") != pool.add("def"));
}
