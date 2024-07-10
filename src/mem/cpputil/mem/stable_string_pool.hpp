#pragma once

#include <format>
#include <string_view>
#include <vector>

namespace cpputil::inline v0::mem {

    // View of a string that belongs to a `Stable_string_pool`.
    class Stable_pool_string {
        std::string_view m_view;

        // Constructor only accessible by the pool.
        explicit Stable_pool_string(std::string_view view) noexcept;
    public:
        friend class Stable_string_pool;

        // Retrieve the string view. Valid while the owning pool lives.
        [[nodiscard]] auto view() const noexcept -> std::string_view;

        // Shallow equality comparison. `this` and `other` must belong to the same pool.
        auto operator==(Stable_pool_string const& other) const noexcept -> bool;
    };

    // Paged string pool with stable addresses.
    class Stable_string_pool {
        std::vector<std::vector<char>> m_pages;
        std::size_t                    m_page_size = 256;
    public:
        // Construct a string pool with the given page size.
        [[nodiscard]] static auto with_page_size(std::size_t page_size) -> Stable_string_pool;

        // Retrieve the number of pages.
        [[nodiscard]] auto page_count() const noexcept -> std::size_t;

        // Add `string` to the pool. Returns the pooled copy.
        [[nodiscard]] auto add(std::string_view string) -> Stable_pool_string;
    };

} // namespace cpputil::inline v0::mem

template <>
struct std::formatter<cpputil::v0::mem::Stable_pool_string> : std::formatter<std::string_view> {
    auto format(cpputil::v0::mem::Stable_pool_string const& string, auto& context) const
    {
        return std::formatter<std::string_view>::format(string.view(), context);
    }
};
