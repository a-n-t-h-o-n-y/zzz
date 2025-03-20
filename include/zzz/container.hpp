#pragma once
#include <algorithm>
#include <numeric>
#include <optional>
#include <string>
#include <string_view>
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

}  // namespace zzz
