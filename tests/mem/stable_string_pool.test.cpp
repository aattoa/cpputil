#include <cpputil/mem/stable_string_pool.hpp>
#include <cppunittest/unittest.hpp>

UNITTEST("stable string pool")
{
    auto pool = cpputil::mem::Stable_string_pool::with_page_size(10);

    auto const a = pool.add("hello");
    auto const b = pool.add("world");
    auto const c = pool.add("helloworld");
    REQUIRE_EQUAL(pool.page_count(), 1UZ);

    REQUIRE_EQUAL(a.view(), "hello");
    REQUIRE_EQUAL(b.view(), "world");
    REQUIRE_EQUAL(c.view(), "helloworld");

    REQUIRE_EQUAL((void*)a.view().data(), (void*)c.view().data());
    REQUIRE_EQUAL((void*)(a.view().data() + a.view().size()), (void*)b.view().data());

    (void)pool.add("abcdefghi");
    REQUIRE_EQUAL(pool.page_count(), 2UZ);
    (void)pool.add("0123456789");
    REQUIRE_EQUAL(pool.page_count(), 3UZ);

    // TODO
    // (void)pool.add("ghij");
    // REQUIRE_EQUAL(pool.page_count(), 3UZ);
}
