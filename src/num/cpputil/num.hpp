#pragma once

#include <concepts>
#include <stdexcept>
#include <utility>
#include <limits>

namespace cpputil::inline v0::num {

    template <typename From, typename To>
    concept losslessly_convertible = std::integral<From> and std::integral<To>
                                 and std::in_range<To>(std::numeric_limits<From>::min())
                                 and std::in_range<To>(std::numeric_limits<From>::max());

    struct Safe_cast_invalid_argument : std::invalid_argument {
        Safe_cast_invalid_argument();
    };

    template <std::integral To, std::integral From>
    [[nodiscard]] constexpr auto safe_cast(From const from)
        noexcept(losslessly_convertible<From, To>) -> To
    {
        if constexpr (not losslessly_convertible<From, To>) {
            if (not std::in_range<To>(from)) {
                throw Safe_cast_invalid_argument {};
            }
        }
        return static_cast<To>(from);
    }

} // namespace cpputil::inline v0::num
