#pragma once

#include <string>
#include <optional>

namespace cpputil::inline v0::input {

    // 1: If `prompt` is not null, write it to `stdout`.
    // 2: Read a line of input from `stdin`.
    // Uses `<readline/readline.h>` if it is available.
    [[nodiscard]] auto read_line(char const* prompt = nullptr) -> std::optional<std::string>;

    // Append `line` to the current history list.
    // Uses `<readline/history.h>` if it is available. Otherwise does nothing.
    auto add_history(char const* line) -> void;

} // namespace cpputil::inline v0::input
