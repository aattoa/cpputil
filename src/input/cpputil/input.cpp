#include <cpputil/input.hpp>

// cpputil::input only links against cpputil::io if the readline library is not available.

#if __has_include(<cpputil/io.hpp>)

#include <cpputil/io.hpp>

auto cpputil::input::read_line(char const* const prompt) -> std::optional<std::string>
{
    if (prompt) {
        (void)io::write(stdout, prompt);
        (void)std::fflush(stdout);
    }
    return io::read_line(stdin);
}

auto cpputil::input::add_history(char const* const line) -> void
{
    (void)line; // Do nothing
}

#else

#include <readline/readline.h>
#include <readline/history.h>
#include <cstdlib>
#include <memory>

auto cpputil::input::read_line(char const* const prompt) -> std::optional<std::string>
{
    using Free = decltype([](void* const ptr) { std::free(ptr); }); // NOLINT
    std::unique_ptr<char, Free> const line { ::readline(prompt) };
    return line ? std::optional(line.get()) : std::nullopt;
}

auto cpputil::input::add_history(char const* const line) -> void
{
    ::add_history(line);
}

#endif
