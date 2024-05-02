#include <cpputil/util.hpp>

auto cpputil::abort(std::string_view const message, std::source_location const caller) -> void
{
    std::println(
        stderr,
        "[{}:{}:{}] {}, in function '{}'",
        caller.file_name(),
        caller.line(),
        caller.column(),
        message,
        caller.function_name());
    std::quick_exit(EXIT_FAILURE);
}

auto cpputil::todo(std::source_location const caller) -> void
{
    abort("Unimplemented branch reached", caller);
}

auto cpputil::unreachable(std::source_location const caller) -> void
{
    abort("Unreachable branch reached", caller);
}

auto cpputil::always_assert(bool const value, std::source_location const caller) -> void
{
    if (!value) [[unlikely]] {
        abort("Assertion failed", caller);
    }
}

auto cpputil::trace(std::source_location const caller) -> void
{
    std::println(
        stderr,
        "cpputil::trace: Reached line {} in {}, in function '{}'",
        caller.line(),
        caller.file_name(),
        caller.function_name());
}
