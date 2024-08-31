#pragma once

#include <cpputil/json.hpp>
#include <format>
#include <iterator>
#include <string_view>

namespace cpputil::inline v0::json {

    template <std::output_iterator<char> Out, class Arithmetic>
        requires std::is_arithmetic_v<Arithmetic>
    auto to_json(Out out, Arithmetic const arithmetic) -> Out
    {
        return std::format_to(out, "{}", arithmetic);
    }

    template <std::output_iterator<char> Out>
    auto to_json(Out out, std::string_view const string) -> Out
    {
        return std::format_to(out, "{:?}", string);
    }

    template <std::output_iterator<char> Out>
    auto to_json(Out out, Null) -> Out
    {
        return std::format_to(out, "null");
    }

    template <std::output_iterator<char> Out, class A, class B>
    auto to_json(Out out, std::pair<A, B> const& pair) -> Out
    {
        out    = to_json(out, pair.first);
        *out++ = ':';
        out    = to_json(out, pair.second);
        return out;
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

} // namespace cpputil::inline v0::json
