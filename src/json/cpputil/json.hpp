#pragma once

#include <expected>
#include <format>
#include <iterator>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

namespace cpputil::inline v0::json {

    template <class T>
    concept configuration = requires {
        typename T::Object;
        typename T::Array;
        typename T::String;
        typename T::Number;
        typename T::Boolean;
    };

    template <configuration Config>
    using Value_variant = std::variant<
        std::monostate,
        typename Config::Object,
        typename Config::Array,
        typename Config::String,
        typename Config::Number,
        typename Config::Boolean>;

    template <class Config> // Unconstrained to enable recursive types
    struct Basic_value {
        Value_variant<Config> variant;

        [[nodiscard]] constexpr auto is_null() const noexcept -> bool
        {
            return std::holds_alternative<std::monostate>(variant);
        }

        [[nodiscard]] constexpr auto is_object() const noexcept -> bool
        {
            return std::holds_alternative<typename Config::Object>(variant);
        }

        [[nodiscard]] constexpr auto is_array() const noexcept -> bool
        {
            return std::holds_alternative<typename Config::Array>(variant);
        }

        [[nodiscard]] constexpr auto is_string() const noexcept -> bool
        {
            return std::holds_alternative<typename Config::String>(variant);
        }

        [[nodiscard]] constexpr auto is_number() const noexcept -> bool
        {
            return std::holds_alternative<typename Config::Number>(variant);
        }

        [[nodiscard]] constexpr auto is_boolean() const noexcept -> bool
        {
            return std::holds_alternative<typename Config::Boolean>(variant);
        }

        [[nodiscard]] constexpr auto as_object(this auto&& self) -> decltype(auto)
        {
            return std::get<typename Config::Object>(std::forward<decltype(self)>(self).variant);
        }

        [[nodiscard]] constexpr auto as_array(this auto&& self) -> decltype(auto)
        {
            return std::get<typename Config::Array>(std::forward<decltype(self)>(self).variant);
        }

        [[nodiscard]] constexpr auto as_string(this auto&& self) -> decltype(auto)
        {
            return std::get<typename Config::String>(std::forward<decltype(self)>(self).variant);
        }

        [[nodiscard]] constexpr auto as_number(this auto&& self) -> decltype(auto)
        {
            return std::get<typename Config::Number>(std::forward<decltype(self)>(self).variant);
        }

        [[nodiscard]] constexpr auto as_boolean(this auto&& self) -> decltype(auto)
        {
            return std::get<typename Config::Boolean>(std::forward<decltype(self)>(self).variant);
        }

        constexpr auto clear() noexcept -> void
        {
            variant = std::monostate {};
        }
    };

    struct Default_configuration {
        using Object  = std::unordered_map<std::string, Basic_value<Default_configuration>>;
        using Array   = std::vector<Basic_value<Default_configuration>>;
        using String  = std::string;
        using Number  = double;
        using Boolean = bool;

        static_assert(configuration<Default_configuration>);
    };

    using Value = Basic_value<Default_configuration>;

    template <std::output_iterator<char> Out, class Arithmetic>
        requires std::is_arithmetic_v<Arithmetic>
    auto to_json(Out out, Arithmetic const arithmetic) -> Out
    {
        return std::format_to(out, "{}", arithmetic);
    }

    template <std::output_iterator<char> Out>
    auto to_json(Out out, std::string_view const string) -> Out
    {
        return std::format_to(out, "\"{}\"", string);
    }

    template <std::output_iterator<char> Out>
    auto to_json(Out out, std::monostate) -> Out
    {
        return std::format_to(out, "null");
    }

    template <std::output_iterator<char> Out, std::input_iterator It, std::sentinel_for<It> End>
    auto range_to_json(Out out, char const open, char const close, It it, End const end) -> Out
    {
        *out++ = open;
        if (it != end) {
            out = to_json(out, *it);
            while (++it != end) {
                *out++ = ',';
                out    = to_json(out, *it);
            }
        }
        *out++ = close;
        return out;
    }

    template <std::output_iterator<char> Out, class A, class B>
    auto to_json(Out out, std::pair<A, B> const& pair) -> Out
    {
        out    = to_json(out, pair.first);
        *out++ = ':';
        out    = to_json(out, pair.second);
        return out;
    }

    template <std::output_iterator<char> Out, class... Ts>
    auto to_json(Out out, std::vector<Ts...> const& vector) -> Out
    {
        return range_to_json(out, '[', ']', vector.begin(), vector.end());
    }

    template <std::output_iterator<char> Out, class... Ts>
    auto to_json(Out out, std::unordered_map<Ts...> const& map) -> Out
    {
        return range_to_json(out, '{', '}', map.begin(), map.end());
    }

    template <std::output_iterator<char> Out, configuration Config>
    auto to_json(Out out, Basic_value<Config> const& value) -> Out
    {
        return std::visit(
            [&](auto const& alternative) { return to_json(out, alternative); }, value.variant);
    }

    template <configuration Config = Default_configuration>
    [[nodiscard]] auto encode(Basic_value<Config> const& value) -> std::string
    {
        std::string output;
        to_json(std::back_inserter(output), value);
        return output;
    }

    struct Parse_error {};

    template <configuration Config = Default_configuration>
    [[nodiscard]] auto decode(std::string_view) -> std::expected<Basic_value<Config>, Parse_error>;

} // namespace cpputil::inline v0::json
