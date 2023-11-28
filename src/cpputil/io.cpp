#include <cpputil/io.hpp>
#include <utility>
#include <cassert>

// NOLINTBEGIN(cert-err33-c, cppcoreguidelines-owning-memory, readability-use-anyofallof)

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
        std::fclose(m_file);
        m_file = nullptr;
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
    if (!file) {
        return false;
    }
    for (char const character : string) {
        if (std::fputc(character, file) == EOF) {
            return false;
        }
    }
    return true;
}

auto cpputil::io::write_line(std::FILE* const file, std::string_view const string) -> bool
{
    return write(file, string) && (std::fputc('\n', file) != EOF);
}

// NOLINTEND(cert-err33-c, cppcoreguidelines-owning-memory, readability-use-anyofallof)
