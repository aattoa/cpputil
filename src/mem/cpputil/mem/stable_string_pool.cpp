#include <cpputil/mem/stable_string_pool.hpp>
#include <ranges>

auto cpputil::mem::Stable_string_pool::with_page_size(std::size_t const page_size)
    -> Stable_string_pool
{
    Stable_string_pool pool;
    pool.m_page_size = page_size;
    return pool;
}

auto cpputil::mem::Stable_string_pool::page_count() const noexcept -> std::size_t
{
    return m_pages.size();
}

auto cpputil::mem::Stable_string_pool::add(std::string_view const string) -> std::string_view
{
    for (const auto& page : m_pages) {
        if (auto const subrange = std::ranges::search(page, string)) {
            return std::string_view(subrange);
        }
    }

    auto const has_enough_space = [size = string.size()](std::vector<char> const& page) {
        return size <= (page.capacity() - page.size());
    };
    auto page_it = std::ranges::find_if(m_pages, has_enough_space);

    if (page_it == m_pages.end()) {
        std::vector<char> new_page;
        new_page.reserve(std::max(m_page_size, string.size()));
        page_it = m_pages.insert(m_pages.cend(), std::move(new_page));
    }

    auto const it = page_it->insert_range(page_it->cend(), string);
    return { std::to_address(it), string.size() };
}
