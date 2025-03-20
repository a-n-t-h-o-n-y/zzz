#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace zzz {

/// Get a single line of text from \p is, return nullopt when nothing to read.
[[nodiscard]] inline auto getline(std::istream& is, char delimiter = '\n')
    -> std::optional<std::string>
{
    auto result = std::string{};
    if (std::getline(is, result, delimiter))
        return result;
    else
        return std::nullopt;
}

/// Print out each element of an iterable \p x to \p os, surrounded by {} and ,
/// delimiters.
template <typename Iterable>
auto print(std::ostream& os, Iterable const& x) -> std::ostream&
{
    using std::begin;
    using std::end;

    os << "{ ";
    auto it = begin(x);
    auto it_end = end(x);

    for (; it != it_end; ++it) {
        os << *it;
        if (std::next(it) != it_end) { os << ", "; }
    }
    os << " }";
    return os;
}

}  // namespace zzz