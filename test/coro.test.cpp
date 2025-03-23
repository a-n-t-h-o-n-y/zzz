#include <ranges>
#include <vector>

#include <zzz/coro.hpp>

#define TEST_MAIN
#include <zzz/test.hpp>

inline auto numbers(int begin, int end) -> zzz::Generator<int>
{
    for (int i = begin; i < end; ++i) {
        co_yield i;
    }
}

TEST(simple_generator)
{
    {  // l-value generator object
        auto gen = numbers(0, 5);
        auto it = gen.begin();
        ASSERT(it != gen.end());
        ASSERT(*it == 0);
        ++it;
        ASSERT(it != gen.end());
        ASSERT(*it == 1);
        ++it;
        ASSERT(it != gen.end());
        ASSERT(*it == 2);
        ++it;
        ASSERT(it != gen.end());
        ASSERT(*it == 3);
        ++it;
        ASSERT(it != gen.end());
        ASSERT(*it == 4);
        ++it;
        ASSERT(it == gen.end());
    }

    {  // Temporary generator object
        auto count = 0;
        for (auto i : numbers(0, 5)) {
            ASSERT(i == count++);
        }
    }
}

TEST(filter_with_generator)
{
    {  // Filter out odd numbers on vector
        auto const x = std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto evens = std::views::filter(x, [](int i) { return i % 2 == 0; });

        auto it = evens.begin();
        ASSERT(it != evens.end());
        ASSERT(*it == 2);
        ++it;
        ASSERT(it != evens.end());
        ASSERT(*it == 4);
        ++it;
        ASSERT(it != evens.end());
        ASSERT(*it == 6);
        ++it;
        ASSERT(it != evens.end());
        ASSERT(*it == 8);
        ++it;
        ASSERT(it != evens.end());
        ASSERT(*it == 10);
        ++it;
        ASSERT(it == evens.end());
    }

    {      // Filter out odd numbers on Generator
        {  // l-value Generator
            auto const x = numbers(1, 11);
            for (auto i : std::views::filter(x, [](int i) { return i % 2 == 0; })) {
                ASSERT(i % 2 == 0);
            }
        }
        {  // r-value Generator
            for (auto i :
                 std::views::filter(numbers(1, 11), [](int i) { return i % 2 == 0; })) {
                ASSERT(i % 2 == 0);
            }
        }
        {  // With pipe operator
            for (auto i : numbers(1, 11) |
                              std::views::filter([](int i) { return i % 2 == 0; })) {
                ASSERT(i % 2 == 0);
            }
        }
    }

    {  // No elements
        auto const x = std::vector<int>{};
        auto evens = std::views::filter(x, [](int i) { return i % 2 == 0; });
        ASSERT(evens.begin() == evens.end());
    }

    {  // All elements
        auto const x = std::vector{1, 3, 5, 7, 9};
        auto odds = std::views::filter(x, [](int i) { return i % 2 != 0; });
        ASSERT(odds.begin() != odds.end());
    }

    {  // Modify values in-place
        auto x = std::vector<int>{1, 2, 3, 4, 5};
        for (auto&& i : std::views::filter(x, [](int i) { return i % 2 == 0; })) {
            i = 10;
        }
        ASSERT(x[0] == 1);
        ASSERT(x[1] == 10);
        ASSERT(x[2] == 3);
        ASSERT(x[3] == 10);
        ASSERT(x[4] == 5);
    }
}
