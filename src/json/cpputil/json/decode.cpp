#include <cpputil/json/decode.hpp>

auto cpputil::json::Position::advance_with(char const character) noexcept -> void
{
    if (character == '\n') {
        ++line, column = 0;
    }
    else {
        ++column;
    }
}

auto cpputil::json::Parse_error::string(Parse_error::Type const type) -> std::string_view
{
    // clang-format off
    using enum Parse_error::Type;
    switch (type) {
    case end_of_input:         return "end of input";
    case unterminated_string:  return "unterminated string";
    case unimplemented_escape: return "unimplemented escape";
    case invalid_escape:       return "invalid escape";
    case missing_escape:       return "missing escape";
    case missing_colon:        return "missing colon";
    case missing_list_end:     return "missing list end";
    case missing_object_end:   return "missing object end";
    case unexpected_character: return "unexpected character";
    default:                   return "(unknown error type)";
    }
    // clang-format on
}

auto cpputil::json::Parse_state::error(Parse_error::Type const type) const noexcept
    -> std::unexpected<Parse_error>
{
    return std::unexpected { Parse_error { position, type } };
}

auto cpputil::json::Parse_state::current() const -> char
{
    return string.front();
}

auto cpputil::json::Parse_state::advance() -> void
{
    position.advance_with(string.front());
    string.remove_prefix(1);
}

auto cpputil::json::Parse_state::extract(std::string_view const prefix) -> bool
{
    if (string.starts_with(prefix)) {
        string.remove_prefix(prefix.size());
        position.column += prefix.size();
        return true;
    }
    return false;
}

auto cpputil::json::Parse_state::extract(char const prefix) -> bool
{
    if (string.starts_with(prefix)) {
        string.remove_prefix(1);
        ++position.column;
        return true;
    }
    return false;
}

static constexpr auto is_space(char const c) noexcept -> bool
{
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

auto cpputil::json::Parse_state::skip_whitespace() -> void
{
    while (!string.empty() && is_space(current())) {
        advance();
    }
}

auto cpputil::json::extract_escape(Parse_state& state) -> std::expected<char, Parse_error>
{
    if (state.string.empty()) {
        return state.error(Parse_error::Type::missing_escape);
    }
    // clang-format off
    switch (state.current()) {
    case '"':  state.advance(); return '"';
    case '/':  state.advance(); return '/';
    case '\\': state.advance(); return '\\';
    case 'b':  state.advance(); return '\b';
    case 'f':  state.advance(); return '\f';
    case 'n':  state.advance(); return '\n';
    case 'r':  state.advance(); return '\r';
    case 't':  state.advance(); return '\t';
    case 'u':
        return state.error(Parse_error::Type::unimplemented_escape);
    default:
        return state.error(Parse_error::Type::invalid_escape);
    }
    // clang-format on
}

#include <cstdlib>
#include <string>
#include <string_view>

// Absolutely terrible workaround for missing from_chars floating point overloads.
auto cpputil::json::from_chars_workaround(std::string_view string, double& value)
    -> std::from_chars_result
{
    // Avoid huge allocations.
    if (string.size() > 100) {
        string.remove_suffix(string.size() - 100);
    }

    // strtod requires a null terminated string, so one has to be allocated.
    std::string const buffer(string);

    char* ptr {};
    value = std::strtod(buffer.data(), &ptr);

    if (ptr == string.data() || ptr == nullptr) {
        return { string.data(), std::errc::invalid_argument };
    }
    if (value == HUGE_VAL || value == HUGE_VALF || value == HUGE_VALL) {
        return { string.data(), std::errc::result_out_of_range };
    }
    return { string.data() + (ptr - buffer.data()), std::errc {} };
}
