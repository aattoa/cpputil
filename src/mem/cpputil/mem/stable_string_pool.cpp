#include <cpputil/mem/stable_string_pool.hpp>
#include <ranges>

cpputil::mem::Stable_pool_string::Stable_pool_string(std::string_view const view) noexcept
    : m_view { view }
{}

auto cpputil::mem::Stable_pool_string::view() const noexcept -> std::string_view
{
    return m_view;
}

auto cpputil::mem::Stable_pool_string::operator==(Stable_pool_string const& other) const noexcept
    -> bool
{
    return m_view.data() == other.m_view.data() && m_view.size() == other.m_view.size();
}

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

auto cpputil::mem::Stable_string_pool::add(std::string_view const string) -> Stable_pool_string
{
    for (auto const& page : m_pages) {
        if (auto const subrange = std::ranges::search(page, string)) {
            return Stable_pool_string(std::string_view(subrange));
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
    return Stable_pool_string(std::string_view(std::to_address(it), string.size()));
}
