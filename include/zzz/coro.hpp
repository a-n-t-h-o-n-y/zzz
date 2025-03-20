#pragma once
#include <cassert>
#include <coroutine>
#include <exception>
#include <functional>
#include <iterator>
#include <memory>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace zzz {

/**
 * @brief Generator coroutine type that yields values of type T.
 */
template <typename T>
class Generator {
   public:
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        using value_type = std::remove_reference_t<T>;
        using reference_type = std::conditional_t<std::is_reference_v<T>, T, T&>;
        using pointer_type = value_type*;

        pointer_type value_{nullptr};
        std::exception_ptr exception{nullptr};

        auto get_return_object() { return Generator{handle_type::from_promise(*this)}; }

        auto initial_suspend() noexcept { return std::suspend_always{}; }

        auto final_suspend() noexcept { return std::suspend_always{}; }

        template <typename U = T,
                  std::enable_if_t<!std::is_rvalue_reference_v<U>, int> = 0>
        auto yield_value(std::remove_reference_t<T>& v) noexcept
        {
            value_ = std::addressof(v);
            return std::suspend_always{};
        }

        auto yield_value(std::remove_reference_t<T>&& v) noexcept
        {
            value_ = std::addressof(v);
            return std::suspend_always{};
        }

        void return_void() noexcept {}

        void unhandled_exception() { exception = std::current_exception(); }

        reference_type value() const noexcept
        {
            return static_cast<reference_type>(*value_);
        }

        // Don't allow any use of 'co_await' inside the generator coroutine
        template <typename U>
        std::suspend_never await_transform(U&&) = delete;
    };

   public:
    explicit Generator(handle_type h) : handle_(h) {}

    Generator(Generator const&) = delete;
    auto operator=(Generator const&) -> Generator& = delete;

    Generator(Generator&& other) noexcept : handle_(other.handle_)
    {
        other.handle_ = nullptr;
    }

    auto operator=(Generator&& other) noexcept -> Generator&
    {
        if (this != &other) {
            if (handle_) handle_.destroy();
            handle_ = other.handle_;
            other.handle_ = nullptr;
        }
        return *this;
    }

    ~Generator()
    {
        if (handle_) { handle_.destroy(); }
    }

    template <bool IsConst>
    class IteratorBase {
       public:
        using iterator_category = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = std::remove_reference_t<T>;
        using reference = std::conditional_t<IsConst, const T, T>;
        using pointer = std::conditional_t<IsConst, const value_type*, value_type*>;

        IteratorBase() noexcept = default;
        explicit IteratorBase(handle_type handle) : handle_(handle)
        {
            if (handle_) {
                handle_.resume();
                if (handle_.done()) { handle_ = nullptr; }
            }
        }

        auto operator++() -> IteratorBase&
        {
            if (handle_ && !handle_.done()) {
                handle_.resume();
                if (handle_.done()) { handle_ = nullptr; }
                else if (handle_.promise().exception) {
                    std::rethrow_exception(handle_.promise().exception);
                }
            }
            return *this;
        }

        auto operator++(int) -> IteratorBase
        {
            IteratorBase tmp = *this;
            ++(*this);
            return tmp;
        }

        auto operator*() const -> reference
        {
            if (!handle_ || handle_.done()) {
                throw std::out_of_range("Generator is exhausted");
            }
            return handle_.promise().value();
        }

        auto operator->() const -> pointer { return std::addressof(operator*()); }

        auto operator==(const IteratorBase& other) const noexcept -> bool
        {
            return handle_ == other.handle_;
        }

        auto operator!=(const IteratorBase& other) const noexcept -> bool
        {
            return !(*this == other);
        }

       private:
        handle_type handle_;
    };

    using Iterator = IteratorBase<false>;
    using ConstIterator = IteratorBase<true>;

   public:
    auto begin() -> Iterator { return Iterator{handle_}; }
    auto begin() const -> ConstIterator { return ConstIterator{handle_}; }
    auto cbegin() const -> ConstIterator { return ConstIterator{handle_}; }

    auto end() -> Iterator { return Iterator{}; }
    auto end() const -> ConstIterator { return ConstIterator{}; }
    auto cend() const -> ConstIterator { return ConstIterator{}; }

   private:
    handle_type handle_;
};

// -------------------------------------------------------------------------------------

/**
 * Pairs elements with an index.
 * @param range The range to enumerate.
 * @param init The initial index, not a param to skip elements.
 */
template <std::ranges::range R>
[[nodiscard]]
auto enumerate(R& range, std::size_t init = 0)
    -> Generator<std::pair<std::size_t, std::ranges::range_reference_t<R>>>
{
    for (auto&& e : std::forward<R>(range)) {
        co_yield {init++, e};
    }
}

template <std::ranges::range R>
[[nodiscard]]
auto enumerate(R&& range, std::size_t init = 0)
    -> Generator<std::pair<std::size_t, std::ranges::range_reference_t<R>>>
{
    R rng = std::forward<R>(range);
    for (auto&& e : rng) {
        co_yield {init++, e};
    }
}

// -------------------------------------------------------------------------------------

/**
 * Zips multiple ranges into a single range of tuple references.
 * @note All ranges must have the same length, otherwise the generator will cause UB.
 */
template <std::ranges::range... Rs>
[[nodiscard]]
auto zip(Rs&&... ranges) -> Generator<std::tuple<std::ranges::range_reference_t<Rs>...>>
{
    static_assert(sizeof...(Rs) > 0, "At least one range must be provided");
    auto iterators = std::make_tuple(std::begin(ranges)...);
    auto ends = std::make_tuple(std::end(ranges)...);

    while (std::get<0>(iterators) != std::get<0>(ends)) {
        co_yield std::apply([](auto&&... iters) { return std::tie(*iters...); },
                            iterators);
        std::apply([](auto&&... iters) { ((++iters), ...); }, iterators);
    }
}

// -------------------------------------------------------------------------------------

/**
 * Generates a half open range of integers. [start, stop)
 * @details Returns empty generator if step == 0.
 */
[[nodiscard]] inline auto iota(int start, int stop, int step = 1) -> Generator<int>
{
    if (step > 0) {
        for (int i = start; i < stop; i += step) {
            co_yield i;
        }
    }
    else if (step < 0) {
        for (int i = start; i > stop; i += step) {
            co_yield i;
        }
    }
}

// -------------------------------------------------------------------------------------

/**
 * Filters elements from a range.
 * @details The predicate \p pred is called for each element in \p range. If the
 * predicate returns true, the element is yielded.
 */
template <std::ranges::range R, typename Pred>
[[nodiscard]]
auto filter(R&& range, Pred&& pred) -> Generator<std::ranges::range_reference_t<R>>
{
    for (auto&& e : range) {
        if (std::invoke(pred, e)) { co_yield e; }
    }
}

// -------------------------------------------------------------------------------------

/**
 * Maps elements from a range.
 * @details The function \p func is called for each element in \p range. The result
 * of the function is yielded.
 */
template <std::ranges::range R, typename Func>
[[nodiscard]]
auto map(R&& range, Func&& func)
    -> Generator<std::invoke_result_t<Func, std::ranges::range_reference_t<R>>>
{
    for (auto&& e : range) {
        co_yield std::invoke(func, e);
    }
}

// -------------------------------------------------------------------------------------

/**
 * Takes the first \p n elements from a range.
 */
template <std::ranges::range R>
[[nodiscard]]
auto take(R&& range, std::size_t n) -> Generator<std::ranges::range_reference_t<R>>
{
    auto it = std::begin(range);
    auto end = std::end(range);

    for (std::size_t i = 0; i < n && it != end; ++i, ++it) {
        co_yield *it;
    }
}

// -------------------------------------------------------------------------------------

/**
 * Takes elements from a range while the predicate is true.
 * @details The predicate \p pred is called for each element in \p range. If the
 * predicate returns true, the element is yielded. If the predicate returns false,
 * the generator stops.
 */
template <std::ranges::range R, typename Pred>
[[nodiscard]]
auto take_while(R&& range, Pred&& pred) -> Generator<std::ranges::range_reference_t<R>>
{
    for (auto&& e : range) {
        if (!std::invoke(pred, e)) { break; }
        co_yield e;
    }
}

// -------------------------------------------------------------------------------------

/**
 * Drops the first \p n elements from a range.
 */
template <std::ranges::range R>
[[nodiscard]]
auto drop(R&& range, std::size_t n) -> Generator<std::ranges::range_reference_t<R>>
{
    auto it = std::begin(range);
    auto end = std::end(range);

    for (std::size_t i = 0; i < n && it != end; ++i, ++it) {}

    while (it != end) {
        co_yield *it++;
    }
}

// -------------------------------------------------------------------------------------

/**
 * Drops elements from a range while the predicate is true.
 * @details The predicate \p pred is called for each element in \p range. If the
 * predicate returns true, the element is skipped. If the predicate returns false,
 * the element is yielded and the generator stops skipping elements.
 */
template <std::ranges::range R, typename Pred>
[[nodiscard]]
auto drop_while(R&& range, Pred&& pred) -> Generator<std::ranges::range_reference_t<R>>
{
    auto it = std::begin(range);
    auto end = std::end(range);

    while (it != end && std::invoke(pred, *it)) {
        ++it;
    }

    while (it != end) {
        co_yield *it++;
    }
}

// -------------------------------------------------------------------------------------

/**
 * Repeats a single element \p n times.
 * @details Takes a copy of \p value.
 */
template <typename T>
[[nodiscard]]
auto repeat(T value, std::size_t n) -> Generator<T>
{
    for (std::size_t i = 0; i < n; ++i) {
        co_yield value;
    }
}

// -------------------------------------------------------------------------------------

}  // namespace zzz