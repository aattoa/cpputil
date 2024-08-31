#pragma once

#include <cpputil/json.hpp>
#include <cpputil/json/decode.hpp>
#include <format>

template <>
struct std::formatter<cpputil::v0::json::Null> {
    static constexpr auto parse(auto& context)
    {
        return context.begin();
    }

    template <class Context>
    static auto format(cpputil::v0::json::Null const&, Context& context) -> Context::iterator
    {
        return std::format_to(context.out(), "null");
    }
};

template <>
struct std::formatter<cpputil::v0::json::Position> {
    static constexpr auto parse(auto& context)
    {
        return context.begin();
    }

    template <class Context>
    static auto format(cpputil::v0::json::Position const& position, Context& context)
        -> Context::iterator
    {
        return std::format_to(context.out(), "({}:{})", position.line, position.column);
    }
};

template <>
struct std::formatter<cpputil::v0::json::Parse_error> {
    static constexpr auto parse(auto& context)
    {
        return context.begin();
    }

    template <class Context>
    static auto format(cpputil::v0::json::Parse_error const& error, Context& context)
        -> Context::iterator
    {
        auto const message = cpputil::v0::json::Parse_error::string(error.type);
        return std::format_to(context.out(), "{}: {}", error.position, message);
    }
};

template <cpputil::v0::json::configuration Config>
struct std::formatter<cpputil::v0::json::Basic_value<Config>> {
    static constexpr auto parse(auto& context)
    {
        return context.begin();
    }

    template <class Context>
    static auto format(cpputil::v0::json::Basic_value<Config> const& value, Context& context)
        -> Context::iterator
    {
        auto const visitor = [&](auto const& alternative) {
            return std::format_to(context.out(), "{}", alternative);
        };
        return std::visit(visitor, value.variant);
    }
};
