#ifndef ZZZ_ZZZ_HPP
#define ZZZ_ZZZ_HPP
#include <algorithm>
#include <array>
#include <cctype>
#include <cstddef>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

namespace zzz {

/// Return true if \p x has no elements, false otherwise.
template <typename Container>
[[nodiscard]] auto is_empty(Container const& x) -> bool
{
    return x.empty();
}

/// Return the first element of \p x; return std::nullopt if \p x is empty.
template <template <typename...> typename Container, typename T>
[[nodiscard]] auto head(Container<T> const& x) -> std::optional<T>
{
    return is_empty(x) ? std::nullopt : std::optional<T>{x.front()};
}

/// Return the last element of \p x; return std::nullopt if \p x is empty.
template <template <typename...> typename Container, typename T>
[[nodiscard]] auto tail(Container<T> const& x) -> std::optional<T>
{
    return is_empty(x) ? std::nullopt : std::optional<T>{x.back()};
}

/// Return the value associated with \p key in \p x; return nullopt if not found
template <template <typename...> typename Mappable, typename K, typename V>
[[nodiscard]] auto lookup(Mappable<K, V> const& x, K const& key) -> std::optional<V>
{
    auto const at = x.find(key);
    if (at == std::cend(x))
        return std::nullopt;
    else
        return at->second;
}

/// Return iterator to first occurance of \p y in \p x; otherwise end iter.
[[nodiscard]] inline auto find(std::string_view x, char y)
    -> std::string_view::const_iterator
{
    return std::find(std::cbegin(x), std::cend(x), y);
}

/// Return iterator to first occurance of \p segment in \p x; otherwise end iter
[[nodiscard]] inline auto find(std::string_view x, std::string_view segment)
    -> std::string_view::const_iterator
{
    return std::search(std::cbegin(x), std::cend(x), std::cbegin(segment),
                       std::cend(segment));
}

/// Return iterator to first occurance of \p y in \p x; otherwise end iter.
template <typename T>
[[nodiscard]] auto find(std::vector<T> const& x, T const& y) ->
    typename std::vector<T>::const_iterator
{
    return std::find(std::cbegin(x), std::cend(x), y);
}

/// Return true if there is at least one occurance of \p y in \p x.
template <typename T>
[[nodiscard]] auto contains(std::vector<T> const& x, T const& y) -> bool
{
    return find(x, y) != std::cend(x);
}

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

/// Partial application of contains. Stores a copy of \p x in result lambda.
template <typename Container>
[[nodiscard]] auto contains(Container const& x)
{
    return [x](auto y) { return contains(x, y); };
}

/// Merge multiple containers, with cooresponding elements stored in a tuple.
/** Throws std::runtime_error if \p xs... are not all the same length. */
template <typename... Ts>
[[nodiscard]] auto zip(std::vector<Ts> const&... xs) -> std::vector<std::tuple<Ts...>>
{
    static_assert(sizeof...(Ts) > 1, "zzz::zip(...) needs at least 2 ranges.");

    auto const [min, max] = std::minmax({xs.size()...});
    if (min != max)
        throw std::runtime_error{"zzz::zip(...) elements must be equal size."};

    auto result = std::vector<std::tuple<Ts...>>{};

    for (auto i = std::size_t{0}; i < min; ++i)
        result.emplace_back(xs[i]...);

    return result;
}

/// Merge multiple containers, with cooresponding elements stored in a tuple.
/** Ts... types must be default constructible. */
template <typename... Ts, std::size_t N>
[[nodiscard]] auto zip(std::array<Ts, N> const&... xs)
    -> std::array<std::tuple<Ts...>, N>
{
    static_assert(sizeof...(Ts) > 1, "zzz::zip(...) needs at least 2 ranges.");

    auto result = std::array<std::tuple<Ts...>, N>{};

    for (auto i = std::size_t{0}; i < N; ++i)
        result[i] = std::tuple{xs[i]...};

    return result;
}

template <typename T>
[[nodiscard]] auto zip_with_index(std::vector<T> const& x, std::size_t begin = 0)
    -> std::vector<std::pair<std::size_t, T>>
{
    auto indices = std::vector<std::size_t>(x.size(), 0);
    std::iota(std::begin(indices), std::end(indices), begin);

    auto result = std::vector<std::pair<std::size_t, T>>{};

    for (auto i = std::size_t{0}; i < x.size(); ++i)
        result.emplace_back(indices[i], x[i]);

    return result;
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

/// Apply \p func to each element in \p x, storing the result in a new container
template <typename F, typename T>
[[nodiscard]] auto map(F&& func, std::vector<T> const& x)
    -> std::vector<std::invoke_result_t<F, T>>
{
    auto result = std::vector<std::invoke_result_t<F, T>>{};
    std::transform(std::cbegin(x), std::cend(x), std::back_inserter(result),
                   std::forward<F>(func));
    return result;
}

/// Apply \p func to each element in \p x, storing the result in a new container
/** The result type of F(T) must be default constructible. */
template <typename F, typename T, std::size_t N>
[[nodiscard]] auto map(F&& func, std::array<T, N> const& x)
    -> std::array<std::invoke_result_t<F, T>, N>
{
    auto result = std::array<std::invoke_result_t<F, T>, N>{};
    std::transform(std::cbegin(x), std::cend(x), std::begin(result),
                   std::forward<F>(func));
    return result;
}

/// Apply \p func to each char in \p x, storing the result in a new std::string.
template <typename F>
[[nodiscard]] auto map(F&& func, std::string_view x) -> std::string
{
    auto result = std::string{};
    std::transform(std::cbegin(x), std::cend(x), std::back_inserter(result),
                   std::forward<F>(func));
    return result;
}

/// Apply \p func to the value in \p x, if one exists.
template <typename F, typename T>
[[nodiscard]] auto map(F&& func, std::optional<T> const& x)
    -> std::optional<std::invoke_result_t<F, T>>
{
    if (x.has_value())
        return std::forward<F>(func)(*x);
    else
        return std::nullopt;
}

/// Apply the overload set \p func to \p x with std::visit and return result.
template <typename F, typename... Ts>
[[nodiscard]] auto map(F&& func, std::variant<Ts...> const& x)
{
    return std::visit(std::forward<F>(func), x);
}

/// Return \p x in all uppercase.
[[nodiscard]] auto uppercase(std::string_view x) -> std::string
{
    return map([](char c) { return static_cast<char>(std::toupper(c)); }, x);
}

/// Return \p x in all lowercase.
[[nodiscard]] auto lowercase(std::string_view x) -> std::string
{
    return map([](char c) { return static_cast<char>(std::tolower(c)); }, x);
}

/// Apply result of F(U, T) to each element in \p x and return the result.
template <typename F, typename T, typename U>
[[nodiscard]] auto reduce(F&& func, U initial, std::vector<T> const& x) -> U
{
    return std::accumulate(std::cbegin(x), std::cend(x), initial,
                           std::forward<F>(func));
}

/// Apply result of F(U, T) to each element in \p x and return the result.
template <typename F, typename T, typename U, std::size_t N>
[[nodiscard]] auto reduce(F&& func, U initial, std::array<T, N> const& x) -> U
{
    return std::accumulate(std::cbegin(x), std::cend(x), initial,
                           std::forward<F>(func));
}

/// Apply result of F(U, char) to each element in \p x and return the result.
template <typename F, typename U>
[[nodiscard]] auto reduce(F&& func, U initial, std::string_view x) -> U
{
    return std::accumulate(std::cbegin(x), std::cend(x), initial,
                           std::forward<F>(func));
}

/// Partial application of reduce.
template <typename F>
[[nodiscard]] auto reduce(F&& func)
{
    return [func](auto initial, auto const& x) { return reduce(func, initial, x); };
}

/// Partial application of reduce.
template <typename F, typename U>
[[nodiscard]] auto reduce(F&& func, U initial)
{
    return [func, initial](auto const& x) { return reduce(func, initial, x); };
}

/// Print out each element of \p x to \p os, surrounded by {} and , delimiters.
template <typename T>
auto print(std::ostream& os, std::vector<T> const& x) -> std::ostream&
{
    os << "{ ";
    for (std::size_t i = 0; i < x.size(); ++i) {
        os << x[i];
        if (i < x.size() - 1) { os << ", "; }
    }
    os << " }";
    return os;
}

/// Print out each element of \p x to \p os, surrounded by {} and , delimiters.
template <typename T, std::size_t N>
auto print(std::ostream& os, std::array<T, N> const& x) -> std::ostream&
{
    os << "{ ";
    for (std::size_t i = 0; i < N; ++i) {
        os << x[i];
        if (i < N - 1) { os << ", "; }
    }
    os << " }";
    return os;
}

/// Return true if \p c is a whitespace character.
[[nodiscard]] inline auto is_whitespace(char c) -> bool { return std::isspace(c) != 0; }

/// Return true if \p c is an alphabetic character.
[[nodiscard]] inline auto is_alphabetic(char c) -> bool { return std::isalpha(c) != 0; }

/// Return true if \p c is a digit character.
[[nodiscard]] inline auto is_digit(char c) -> bool { return std::isdigit(c) != 0; }

/// Return true if \p c is a hexadecimanl digit character.
[[nodiscard]] inline auto is_hexidecimal_digit(char c) -> bool
{
    return std::isxdigit(c) != 0;
}

/// Return true if \p c is a control character.
[[nodiscard]] inline auto is_control(char c) -> bool { return std::iscntrl(c) != 0; }

/// Return true if \p c is a control character.
[[nodiscard]] inline auto is_graphical(char c) -> bool { return std::isgraph(c) != 0; }

/// Return true if \p c is a blank character.
[[nodiscard]] inline auto is_blank(char c) -> bool { return std::isblank(c) != 0; }

/// Return true if \p c is an alphanumeric character.
[[nodiscard]] inline auto is_alphanumeric(char c) -> bool
{
    return std::isalnum(c) != 0;
}

/// Return true if \p c is a lowercase character.
[[nodiscard]] inline auto is_lowercase(char c) -> bool { return std::islower(c) != 0; }

/// Return true if \p c is an uppercase character.
[[nodiscard]] inline auto is_uppercase(char c) -> bool { return std::isupper(c) != 0; }

/// Return true if \p c is a punctuation character.
[[nodiscard]] inline auto is_punctuation(char c) -> bool
{
    return std::ispunct(c) != 0;
}

/// Return true if \p c is a printable character.
[[nodiscard]] inline auto is_printable(char c) -> bool { return std::isprint(c) != 0; }

}  // namespace zzz
#endif  // ZZZ_ZZZ_HPP
