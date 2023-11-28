#pragma once

#include <cstdio>
#include <string>
#include <optional>

namespace cpputil::io {

    // Lightweight owning wrapper for a C file stream. Does nothing on its own.
    class [[nodiscard]] File {
        std::FILE* m_file;
        explicit File(std::FILE*) noexcept;
    public:
        File(File const&)           = delete;
        auto operator=(File const&) = delete;

        File(File&&) noexcept;
        auto operator=(File&&) noexcept -> File&;

        ~File();

        // Closes the file if it is open. This is called by the
        // destructor, so a manual call is usually unnecessary.
        auto close() -> void;

        [[nodiscard]] explicit operator bool() const noexcept;
        [[nodiscard]] auto is_open() const noexcept -> bool;
        [[nodiscard]] auto get() const noexcept -> std::FILE*;

        static auto open_read(char const* path) -> File;
        static auto open_write(char const* path) -> File;
        static auto open_append(char const* path) -> File;
        static auto take_ownership(std::FILE* file) -> File;
    };

    // Attempt to read a line from `file` to `output`.
    // If the read fails, `output` remains unmodified.
    [[nodiscard]] auto read_line(std::FILE* file, std::string& output) -> bool;

    // Attempt to read a line from `file`.
    [[nodiscard]] auto read_line(std::FILE* file) -> std::optional<std::string>;

    // Attempt to read the contents of `file` to `output`.
    // If the read fails, `output` remains unmodified.
    [[nodiscard]] auto read(std::FILE* file, std::string& output) -> bool;

    // Attempt to read the contents of `file`.
    [[nodiscard]] auto read(std::FILE* file) -> std::optional<std::string>;

    // Attempt to write `string` to `file`.
    [[nodiscard]] auto write(std::FILE* file, std::string_view string) -> bool;

    // Attempt to write `string` followed by a line feed to `file`.
    [[nodiscard]] auto write_line(std::FILE* file, std::string_view string) -> bool;

} // namespace cpputil::io
