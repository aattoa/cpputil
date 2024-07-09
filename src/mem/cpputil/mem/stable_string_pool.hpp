#pragma once

#include <string_view>
#include <vector>

namespace cpputil::inline v0::mem {

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
        [[nodiscard]] auto add(std::string_view string) -> std::string_view;
    };

} // namespace cpputil::inline v0::mem
