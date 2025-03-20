#pragma once

#include <tuple>
#include <utility>

namespace zzz {

/**
 * Applies \p fn to each element in \p t.
 */
template <typename... Ts, typename Fn>
void for_each(std::tuple<Ts...>& t, Fn&& fn)
{
    std::apply([&](auto&... x) { (std::forward<Fn>(fn)(x), ...); }, t);
}

/**
 * Applies \p fn to each element in \p t. const.
 */
template <typename... Ts, typename Fn>
void for_each(std::tuple<Ts...> const& t, Fn&& fn)
{
    std::apply([&](auto const&... x) { (std::forward<Fn>(fn)(x), ...); }, t);
}

}  // namespace zzz