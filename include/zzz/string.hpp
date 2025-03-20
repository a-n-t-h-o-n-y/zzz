#pragma once
#include <cctype>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

#include "./container.hpp"

namespace zzz {

/// Return true if there is at least one occurance of \p y in \p x.
[[nodiscard]] inline auto contains(std::string_view x, char y) -> bool
{
    return find(x, y) != std::cend(x);
}

/// Return true if there is at least one occurance of \p segment in \p x.
[[nodiscard]] inline auto contains(std::string_view x, std::string_view segment) -> bool
{
    return find(x, segment) != std::cend(x);
}

/// Splits a string on \p delimiter, not including the delimiter in the result.
/** Returns a string_view into the original string. Empty segments allowed.
 *  Empty \p delimiter throws std::runtime_error. */
[[nodiscard]] inline auto split(std::string_view x, std::string_view delimiter = " ")
    -> std::vector<std::string_view>
{
    if (is_empty(delimiter))
        throw std::runtime_error{"zzz::split(...) Delimiter can't be empty."};

    auto begin = std::cbegin(x);
    auto result = std::vector<std::string_view>{};

    while (begin != std::cend(x)) {
        auto const end = std::search(begin, std::cend(x), std::cbegin(delimiter),
                                     std::cend(delimiter));

        // Empty segments are allowed, think csv.
        result.emplace_back(std::addressof(*begin), std::distance(begin, end));

        // A delimiter was found and it is safe to increment length of delimiter
        begin = (end != std::cend(x)) ? std::next(end, delimiter.size()) : end;
    }

    return result;
}

/// Return a std::string_view substring of \p x.
[[nodiscard]] inline auto substring(std::string_view x,
                                    std::size_t begin,
                                    std::size_t length) -> std::string_view
{
    return x.substr(begin, length);
}

/// Return \p x in all uppercase.
[[nodiscard]] auto uppercase(std::string_view x) -> std::string
{
    auto result = std::string{};
    result.reserve(x.size());
    for (auto c : x) {
        result.push_back(static_cast<char>(std::toupper(c)));
    }
    return result;
}

/// Return \p x in all lowercase.
[[nodiscard]] auto lowercase(std::string_view x) -> std::string
{
    auto result = std::string{};
    result.reserve(x.size());
    for (auto c : x) {
        result.push_back(static_cast<char>(std::tolower(c)));
    }
    return result;
}

}  // namespace zzz