#pragma once

#include <string_view>
#include <source_location>

namespace cpputil::inline v0 {

    // Write `message` to `stderr` and terminate program execution with `EXIT_FAILURE`.
    [[noreturn]] auto abort(
        std::string_view message = "Invoked cpputil::abort",
        std::source_location     = std::source_location::current()) -> void;

    // Mark a branch as unfinished. Calls `cpputil::abort` with an appropriate message.
    [[noreturn]] auto todo(std::source_location = std::source_location::current()) -> void;

    // Mark a branch as unreachable. Calls `cpputil::abort` with an appropriate message.
    [[noreturn]] auto unreachable(std::source_location = std::source_location::current()) -> void;

    // Write information about the call site to `stderr`.
    auto trace(std::source_location = std::source_location::current()) -> void;

    // If `condition` is false, call `cpputil::abort` with an appropriate message.
    constexpr auto always_assert(
        bool const                 condition,
        std::source_location const caller = std::source_location::current()) -> void
    {
        if (!condition) [[unlikely]] {
            abort("Assertion failed", caller);
        }
    }

} // namespace cpputil::inline v0
