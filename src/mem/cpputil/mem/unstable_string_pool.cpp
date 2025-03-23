#include <cpputil/mem/unstable_string_pool.hpp>
#include <algorithm>

auto cpputil::mem::Unstable_string_pool::with_capacity(std::size_t const capacity)
    -> Unstable_string_pool
{
    Unstable_string_pool pool;
    pool.m_vector.reserve(capacity);
    return pool;
}

auto cpputil::mem::Unstable_string_pool::add(std::string_view const string) -> Relative_string
{
    std::size_t offset; // NOLINT(cppcoreguidelines-init-variables)

    auto const it = std::search(m_vector.begin(), m_vector.end(), string.begin(), string.end());
    if (it != m_vector.end()) {
        offset = static_cast<std::size_t>(it - m_vector.begin());
    }
    else {
        offset = m_vector.size();
        std::ranges::copy(string, std::back_inserter(m_vector));
    }

    return { .offset = offset, .length = string.length() };
}

auto cpputil::mem::Unstable_string_pool::view(Relative_string const string) const
    -> std::string_view
{
    return std::string_view(m_vector.begin(), m_vector.end()).substr(string.offset, string.length);
}
