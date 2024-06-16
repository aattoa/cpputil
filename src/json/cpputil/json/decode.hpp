#pragma once

#include <cpputil/json.hpp>
#include <cassert>
#include <charconv>
#include <string_view>

namespace cpputil::inline v0::json {

    struct Position {
        std::size_t line {};
        std::size_t column {};

        auto advance_with(char character) noexcept -> void;
    };

    struct Parse_error {
        enum class Type {
            end_of_input,
            unterminated_string,
            unimplemented_escape,
            invalid_escape,
            missing_escape,
            missing_colon,
            missing_list_end,
            missing_object_end,
            unexpected_character,
        };
        Position position;
        Type     type;

        static auto string(Parse_error::Type type) -> std::string_view;
    };

    struct Parse_state {
        Position         position;
        std::string_view string;

        [[nodiscard]] auto error(Parse_error::Type type) const noexcept
            -> std::unexpected<Parse_error>;

        [[nodiscard]] auto current() const -> char;

        [[nodiscard]] auto extract(std::string_view prefix) -> bool;

        [[nodiscard]] auto extract(char prefix) -> bool;

        auto advance() -> void;

        auto skip_whitespace() -> void;
    };

    template <configuration Config>
    constexpr auto make_value = []<class... Args>(Args&&... args) -> Basic_value<Config> {
        return { Value_variant<Config>(std::forward<Args>(args)...) };
    };

    template <configuration Config>
    auto decode(Parse_state& state) -> std::expected<Basic_value<Config>, Parse_error>;

    auto extract_escape(Parse_state& state) -> std::expected<char, Parse_error>;

    template <configuration Config>
    auto extract_string(Parse_state& state) -> std::expected<typename Config::String, Parse_error>
    {
        typename Config::String string;
        for (;;) {
            if (state.string.empty()) {
                return state.error(Parse_error::Type::unterminated_string);
            }

            char character = state.current();
            state.advance();

            if (character == '"') {
                return string;
            }
            if (character == '\\') {
                if (auto const escaped = extract_escape(state)) {
                    character = escaped.value();
                }
                else {
                    return std::unexpected(escaped.error());
                }
            }
            string.push_back(character);
        }
    }

    template <configuration Config>
    auto extract_array(Parse_state& state) -> std::expected<typename Config::Array, Parse_error>
    {
        typename Config::Array array;
        for (;;) {
            state.skip_whitespace();
            if (state.extract(']')) {
                return array;
            }
            if (array.empty() || state.extract(',')) {
                if (auto element = decode<Config>(state)) {
                    array.push_back(std::move(element.value()));
                }
                else {
                    return std::unexpected(element.error());
                }
            }
            else {
                return state.error(Parse_error::Type::unexpected_character);
            }
        }
    }

    template <configuration Config>
    auto extract_key_value_pair(Parse_state& state)
        -> std::expected<typename Config::Object::value_type, Parse_error>
    {
        auto key = extract_string<Config>(state);
        if (!key.has_value()) {
            return std::unexpected(key.error());
        }
        state.skip_whitespace();
        if (!state.extract(':')) {
            return state.error(Parse_error::Type::missing_colon);
        }
        state.skip_whitespace();
        return decode<Config>(state).transform(
            [&key](Basic_value<Config>&& value) -> Config::Object::value_type {
                return { std::move(key.value()), std::move(value) };
            });
    }

    template <configuration Config>
    auto extract_object(Parse_state& state) -> std::expected<typename Config::Object, Parse_error>
    {
        typename Config::Object object;
        for (;;) {
            state.skip_whitespace();
            if (state.extract('}')) {
                return object;
            }
            if ((object.empty() || state.extract(','))
                && (state.skip_whitespace(), state.extract('"')))
            {
                if (auto pair = extract_key_value_pair<Config>(state)) {
                    object.insert(std::move(pair.value()));
                }
                else {
                    return std::unexpected(pair.error());
                }
            }
            else {
                return state.error(Parse_error::Type::unexpected_character);
            }
        }
    }

    // TODO: remove when from_chars floating point overloads are supported.
    auto from_chars_workaround(std::string_view, double&) -> std::from_chars_result;

    template <configuration Config>
    auto decode(Parse_state& state) -> std::expected<Basic_value<Config>, Parse_error>
    {
        state.skip_whitespace();
        if (state.string.empty()) {
            return state.error(Parse_error::Type::end_of_input);
        }
        if (state.extract('"')) {
            return extract_string<Config>(state).transform(make_value<Config>);
        }
        if (state.extract('[')) {
            return extract_array<Config>(state).transform(make_value<Config>);
        }
        if (state.extract('{')) {
            return extract_object<Config>(state).transform(make_value<Config>);
        }
        if (state.extract("true")) {
            return make_value<Config>(true);
        }
        if (state.extract("false")) {
            return make_value<Config>(false);
        }
        if (state.extract("null")) {
            return make_value<Config>(Null {});
        }

        double number; // NOLINT
        state.skip_whitespace();
        auto const [ptr, ec] = from_chars_workaround(state.string, number);

        if (ec == std::errc {}) {
            while (state.string.data() != ptr) {
                state.advance();
            }
            return make_value<Config>(typename Config::Number(number));
        }
        return state.error(Parse_error::Type::unexpected_character);
    }

    template <configuration Config = Default_configuration>
    [[nodiscard]] auto decode(std::string_view const string)
        -> std::expected<Basic_value<Config>, Parse_error>
    {
        Parse_state state { Position {}, string };
        return decode<Config>(state);
    }

} // namespace cpputil::inline v0::json
