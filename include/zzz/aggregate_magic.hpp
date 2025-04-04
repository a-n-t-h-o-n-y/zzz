#pragma once

#include <tuple>
#include <type_traits>

namespace zzz::detail {

template <typename Fn, typename... Args>
auto return_type_impl() -> decltype(std::declval<Fn>()(std::declval<Args>()...));

/// Is the return type of an object of \p Fn with \p Args... applied to it.
template <typename Fn, typename... Args>
using Return_t = decltype(return_type_impl<Fn, Args...>());

template <typename T, typename... Args>
auto test_is_braces_constructible(int)
    -> decltype(void(T{std::declval<Args>()...}), std::true_type{});

template <typename...>
auto test_is_braces_constructible(...) -> std::false_type;

template <typename T, typename... Args>
using is_braces_constructible = decltype(test_is_braces_constructible<T, Args...>(0));

template <typename T, typename... Args>
using has_members = is_braces_constructible<T, Args...>;

struct any_type {
    template <typename T>
    constexpr operator T();
};

template <typename Make_tup, typename T>
constexpr auto to_tuple_impl(Make_tup&& make_tup, T&& object)
{
    using namespace detail;
    using obj_t = std::decay_t<T>;
    using X = any_type;
    if constexpr (has_members<obj_t, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
                              X>{}) {
        auto&& [x0, x1, x2, x3, x4, X5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15] =
            std::forward<T>(object);
        return make_tup(x0, x1, x2, x3, x4, X5, x6, x7, x8, x9, x10, x11, x12, x13, x14,
                        x15);
    }
    else if constexpr (has_members<obj_t, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
                                   X>{}) {
        auto&& [x0, x1, x2, x3, x4, X5, x6, x7, x8, x9, x10, x11, x12, x13, x14] =
            std::forward<T>(object);
        return make_tup(x0, x1, x2, x3, x4, X5, x6, x7, x8, x9, x10, x11, x12, x13,
                        x14);
    }
    else if constexpr (has_members<obj_t, X, X, X, X, X, X, X, X, X, X, X, X, X, X>{}) {
        auto&& [x0, x1, x2, x3, x4, X5, x6, x7, x8, x9, x10, x11, x12, x13] =
            std::forward<T>(object);
        return make_tup(x0, x1, x2, x3, x4, X5, x6, x7, x8, x9, x10, x11, x12, x13);
    }
    else if constexpr (has_members<obj_t, X, X, X, X, X, X, X, X, X, X, X, X, X>{}) {
        auto&& [x0, x1, x2, x3, x4, X5, x6, x7, x8, x9, x10, x11, x12] =
            std::forward<T>(object);
        return make_tup(x0, x1, x2, x3, x4, X5, x6, x7, x8, x9, x10, x11, x12);
    }
    else if constexpr (has_members<obj_t, X, X, X, X, X, X, X, X, X, X, X, X>{}) {
        auto&& [x0, x1, x2, x3, x4, X5, x6, x7, x8, x9, x10, x11] =
            std::forward<T>(object);
        return make_tup(x0, x1, x2, x3, x4, X5, x6, x7, x8, x9, x10, x11);
    }
    else if constexpr (has_members<obj_t, X, X, X, X, X, X, X, X, X, X, X>{}) {
        auto&& [x0, x1, x2, x3, x4, X5, x6, x7, x8, x9, x10] = std::forward<T>(object);
        return make_tup(x0, x1, x2, x3, x4, X5, x6, x7, x8, x9, x10);
    }
    else if constexpr (has_members<obj_t, X, X, X, X, X, X, X, X, X, X>{}) {
        auto&& [x0, x1, x2, x3, x4, X5, x6, x7, x8, x9] = std::forward<T>(object);
        return make_tup(x0, x1, x2, x3, x4, X5, x6, x7, x8, x9);
    }
    else if constexpr (has_members<obj_t, X, X, X, X, X, X, X, X, X>{}) {
        auto&& [x0, x1, x2, x3, x4, X5, x6, x7, x8] = std::forward<T>(object);
        return make_tup(x0, x1, x2, x3, x4, X5, x6, x7, x8);
    }
    else if constexpr (has_members<obj_t, X, X, X, X, X, X, X, X>{}) {
        auto&& [x0, x1, x2, x3, x4, X5, x6, x7] = std::forward<T>(object);
        return make_tup(x0, x1, x2, x3, x4, X5, x6, x7);
    }
    else if constexpr (has_members<obj_t, X, X, X, X, X, X, X>{}) {
        auto&& [x0, x1, x2, x3, x4, X5, x6] = std::forward<T>(object);
        return make_tup(x0, x1, x2, x3, x4, X5, x6);
    }
    else if constexpr (has_members<obj_t, X, X, X, X, X, X>{}) {
        auto&& [x0, x1, x2, x3, x4, X5] = std::forward<T>(object);
        return make_tup(x0, x1, x2, x3, x4, X5);
    }
    else if constexpr (has_members<obj_t, X, X, X, X, X>{}) {
        auto&& [x0, x1, x2, x3, x4] = std::forward<T>(object);
        return make_tup(x0, x1, x2, x3, x4);
    }
    else if constexpr (has_members<obj_t, X, X, X, X>{}) {
        auto&& [x0, x1, x2, x3] = std::forward<T>(object);
        return make_tup(x0, x1, x2, x3);
    }
    else if constexpr (has_members<obj_t, X, X, X>{}) {
        auto&& [x0, x1, x2] = std::forward<T>(object);
        return make_tup(x0, x1, x2);
    }
    else if constexpr (has_members<obj_t, X, X>{}) {
        auto&& [x0, x1] = std::forward<T>(object);
        return make_tup(x0, x1);
    }
    else if constexpr (has_members<obj_t, X>{}) {
        auto&& [x0] = std::forward<T>(object);
        return make_tup(x0);
    }
    else
        return make_tup();
}

}  //  namespace zzz::detail

namespace zzz {

template <typename T>
concept StructType =
    std::is_aggregate_v<std::remove_cvref_t<T>> &&
    !std::is_array_v<std::remove_cvref_t<T>> &&
    !std::is_union_v<std::remove_cvref_t<T>> &&
    std::is_class_v<std::remove_cvref_t<T>> &&
    !std::is_base_of_v<std::tuple<>, std::remove_cvref_t<T>> &&
    requires { typename std::tuple_size<std::remove_cvref_t<T>>::type; } == false;

/**
 *  Return a tuple of copies of each member of the struct.
 *  @details T must be an aggregate type that is not a tuple or array.
 */
template <StructType T>
[[nodiscard]]
constexpr auto to_tuple(T&& object)
{
    return detail::to_tuple_impl(
        [](auto&&... x) { return std::make_tuple(std::forward<decltype(x)>(x)...); },
        std::forward<T>(object));
}

/**
 *  Return a tuple of references to each member of the struct.
 *  @details T must be an aggregate type that is not a tuple or array.
 */
template <StructType T>
[[nodiscard]]
constexpr auto to_ref_tuple(T&& object)
{
    return detail::to_tuple_impl(
        [](auto&&... x) {
            return std::forward_as_tuple(std::forward<decltype(x)>(x)...);
        },
        std::forward<T>(object));
}

}  // namespace zzz