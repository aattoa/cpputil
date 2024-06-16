#pragma once

#include <cpputil/json.hpp>
#include <cpputil/json/decode.hpp>
#include <format>

template <>
struct std::formatter<cpputil::json::Null> {
    static constexpr auto parse(auto& context)
    {
        return context.begin();
    }

    static auto format(cpputil::json::Null const&, auto& context)
    {
        return std::format_to(context.out(), "null");
    }
};

template <>
struct std::formatter<cpputil::json::Position> {
    static constexpr auto parse(auto& context)
    {
        return context.begin();
    }

    static auto format(cpputil::json::Position const& position, auto& context)
    {
        return std::format_to(context.out(), "({}:{})", position.line, position.column);
    }
};

template <>
struct std::formatter<cpputil::json::Parse_error> {
    static constexpr auto parse(auto& context)
    {
        return context.begin();
    }

    static auto format(cpputil::json::Parse_error const& error, auto& context)
    {
        auto const message = cpputil::json::Parse_error::string(error.type);
        return std::format_to(context.out(), "{}: {}", error.position, message);
    }
};

template <cpputil::json::configuration Config>
struct std::formatter<cpputil::json::Basic_value<Config>> {
    template <class Context>
    static constexpr auto parse(Context& context) -> Context::iterator
    {
        return context.begin();
    }

    template <class Context>
    static auto format(cpputil::json::Basic_value<Config> const& value, Context& context)
        -> Context::iterator
    {
        auto const visitor = [&](auto const& alternative) {
            return std::format_to(context.out(), "{}", alternative);
        };
        return std::visit(visitor, value.variant);
    }
};
