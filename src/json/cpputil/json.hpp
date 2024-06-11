#pragma once

#include <expected>
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

    struct Default_configuration;

    template <class Config = Default_configuration>
    struct Value {
        using Variant = std::variant<
            std::monostate,
            typename Config::Object,
            typename Config::Array,
            typename Config::String,
            typename Config::Number,
            typename Config::Boolean>;
        Variant variant;

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
        using Object  = std::unordered_map<std::string, Value<Default_configuration>>;
        using Array   = std::vector<Value<Default_configuration>>;
        using String  = std::string;
        using Number  = double;
        using Boolean = bool;

        static_assert(configuration<Default_configuration>);
    };

    struct Parse_error {};

    template <configuration Configuration = Default_configuration>
    [[nodiscard]] auto decode(std::string_view) -> std::expected<Value<Configuration>, Parse_error>;

    template <configuration Configuration = Default_configuration>
    [[nodiscard]] auto encode(Value<Configuration> const&) -> std::string;

} // namespace cpputil::inline v0::json
