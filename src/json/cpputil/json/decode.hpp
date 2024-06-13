#pragma once

#include <cpputil/json.hpp>
#include <string_view>
#include <iterator>

namespace cpputil::inline v0::json {

    struct Error {};

    template <configuration Config = Default_configuration>
    [[nodiscard]] auto decode(std::string_view) -> std::expected<Basic_value<Config>, Error>;

} // namespace cpputil::inline v0::json
