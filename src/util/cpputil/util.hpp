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

    // If `value` is false, call `cpputil::abort` with an appropriate message.
    auto always_assert(bool value, std::source_location = std::source_location::current()) -> void;

    // Write information about the call site to `stderr`.
    auto trace(std::source_location = std::source_location::current()) -> void;

} // namespace cpputil::inline v0
