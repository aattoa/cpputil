#include <cpputil/num.hpp>
#include <cstdint>

cpputil::num::Safe_cast_invalid_argument::Safe_cast_invalid_argument()
    : invalid_argument("cpputil::num::safe_cast argument out of target range")
{}

static_assert(cpputil::num::losslessly_convertible<std::int8_t, std::int16_t>);
static_assert(cpputil::num::losslessly_convertible<std::int32_t, std::int32_t>);
static_assert(cpputil::num::losslessly_convertible<std::uint8_t, std::int32_t>);
static_assert(cpputil::num::losslessly_convertible<std::uint32_t, std::int64_t>);
static_assert(not cpputil::num::losslessly_convertible<std::int8_t, std::uint8_t>);
static_assert(not cpputil::num::losslessly_convertible<std::uint64_t, std::int8_t>);
static_assert(not cpputil::num::losslessly_convertible<std::int8_t, std::uint64_t>);
static_assert(not cpputil::num::losslessly_convertible<std::int16_t, std::int8_t>);
