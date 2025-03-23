#pragma once

#include <coroutine>
#include <exception>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>

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

}  // namespace zzz