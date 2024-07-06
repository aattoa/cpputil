#pragma once

#include <string_view>
#include <vector>

namespace cpputil::inline v0::mem {

    struct Relative_string {
        std::size_t offset {};
        std::size_t length {};

        auto operator==(Relative_string const&) const -> bool = default;
    };

    // Contiguous string pool with unstable addresses.
    class Unstable_string_pool {
        std::vector<char> m_vector;
    public:
        // Construct a string pool with the given capacity.
        static auto with_capacity(std::size_t capacity) -> Unstable_string_pool;

        // Add `string` to this pool. Returns the pooled relative string.
        auto add(std::string_view string) -> Relative_string;

        // View `string` in this pool.
        [[nodiscard]] auto view(Relative_string string) const -> std::string_view;
    };

} // namespace cpputil::inline v0::mem
