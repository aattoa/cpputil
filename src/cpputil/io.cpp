#include <cpputil/io.hpp>
#include <utility>
#include <algorithm>

auto cpputil::io::detail::Lines_iterator::operator*() const -> std::string const&
{
    return line;
}

auto cpputil::io::detail::Lines_iterator::operator++() -> Lines_iterator&
{
    if (file) {
        line.clear();
        if (!read_line(file, line)) {
            file = nullptr;
        }
    }
    return *this;
}

auto cpputil::io::detail::Lines_iterator::operator++(int) -> Lines_iterator
{
    Lines_iterator copy = *this;
    ++*this;
    return copy;
}

auto cpputil::io::detail::Lines_iterator::operator==(std::default_sentinel_t) const noexcept -> bool
{
    return file == nullptr;
}

auto cpputil::io::detail::Lines::begin() const noexcept -> Lines_iterator
{
    Lines_iterator iterator { .line = {}, .file = file };
    ++iterator; // Prepare for iteration by reading the first line
    return iterator;
}

auto cpputil::io::detail::Lines::end() noexcept -> std::default_sentinel_t
{
    return std::default_sentinel;
}

cpputil::io::File::File(std::FILE* const file) noexcept : m_file { file } {}

cpputil::io::File::File(File&& other) noexcept : m_file { std::exchange(other.m_file, nullptr) } {}

auto cpputil::io::File::operator=(File&& other) noexcept -> File&
{
    if (this != &other) {
        close();
        m_file = std::exchange(other.m_file, nullptr);
    }
    return *this;
}

cpputil::io::File::~File()
{
    close();
}

auto cpputil::io::File::close() -> void
{
    if (m_file) {
        (void)std::fclose(std::exchange(m_file, nullptr));
    }
}

cpputil::io::File::operator bool() const noexcept
{
    return is_open();
}

auto cpputil::io::File::is_open() const noexcept -> bool
{
    return m_file != nullptr;
}

auto cpputil::io::File::get() const noexcept -> std::FILE*
{
    return m_file;
}

auto cpputil::io::File::open_read(char const* const path) -> File
{
    return File { std::fopen(path, "r") };
}

auto cpputil::io::File::open_write(char const* const path) -> File
{
    return File { std::fopen(path, "w") };
}

auto cpputil::io::File::open_append(char const* const path) -> File
{
    return File { std::fopen(path, "a") };
}

auto cpputil::io::File::take_ownership(std::FILE* const file) -> File
{
    return File { file };
}

auto cpputil::io::read_line(std::FILE* const file, std::string& output) -> bool
{
    if (!file) {
        return false;
    }
    auto const original_size = output.size();
    for (;;) {
        int const character = std::fgetc(file);
        if (character == EOF) {
            if (std::ferror(file)) {
                output.resize(original_size);
                return false;
            }
            return output.size() != original_size;
        }
        if (character == '\n') {
            return true;
        }
        output.push_back(static_cast<char>(character));
    }
}

auto cpputil::io::read_line(std::FILE* const file) -> std::optional<std::string>
{
    std::string output;
    return read_line(file, output) ? std::optional(std::move(output)) : std::nullopt;
}

auto cpputil::io::read(std::FILE* const file, std::string& output) -> bool
{
    if (!file) {
        return false;
    }
    auto const original_size = output.size();
    for (;;) {
        int const character = std::fgetc(file);
        if (character == EOF) {
            if (std::ferror(file)) {
                output.resize(original_size);
                return false;
            }
            return true;
        }
        output.push_back(static_cast<char>(character));
    }
}

auto cpputil::io::read(std::FILE* const file) -> std::optional<std::string>
{
    std::string output;
    return read(file, output) ? std::optional(std::move(output)) : std::nullopt;
}

auto cpputil::io::write(std::FILE* const file, std::string_view const string) -> bool
{
    auto const put = [file](char const character) { return std::fputc(character, file) != EOF; };
    return file && std::ranges::all_of(string, put);
}

auto cpputil::io::write_line(std::FILE* const file, std::string_view const string) -> bool
{
    return write(file, string) && (std::fputc('\n', file) != EOF);
}

auto cpputil::io::lines(std::FILE* const file) -> detail::Lines
{
    return { .file = file };
}
