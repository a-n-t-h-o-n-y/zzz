#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include <zzz/coro.hpp>
#define TEST_MAIN
#include <zzz/test.hpp>

TEST(enumerate)
{
    {
        auto words = std::vector<std::string>{"foo", "bar", "baz"};
        auto gen = zzz::enumerate(words);

        // Test begin/end iterator functionality
        auto it = gen.begin();
        auto end = gen.end();

        // First element
        ASSERT(it != end);
        ASSERT((std::pair{0, "foo"} == *it));

        // Second element
        ++it;
        ASSERT(it != end);
        ASSERT((std::pair{1, "bar"} == *it));

        // Third element
        ++it;
        ASSERT(it != end);
        ASSERT((std::pair{2, "baz"} == *it));

        // End of generator
        ++it;
        ASSERT(it == end);
    }

    {  // Empty container
        std::vector<int> empty;
        auto gen = zzz::enumerate(empty);
        ASSERT(gen.begin() == gen.end());
    }

    {  // Single element
        auto single = std::vector{3.14};
        auto gen = zzz::enumerate(single);
        auto it = gen.begin();
        ASSERT(it != gen.end());
        auto [i, val] = *it;
        ASSERT(i == 0);
        ASSERT(val == 3.14);
        ++it;
        ASSERT(it == gen.end());
    }

    {  // Non-copyable elements
        auto ptrs = std::vector<std::unique_ptr<int>>{};
        ptrs.push_back(std::make_unique<int>(10));
        ptrs.push_back(std::make_unique<int>(20));

        auto gen = zzz::enumerate(ptrs);
        auto it = gen.begin();
        {
            auto [i, ptr] = *it;
            ASSERT(i == 0);
            ASSERT(*ptr == 10);
        }
        ++it;
        {
            auto [i, ptr] = *it;
            ASSERT(i == 1);
            ASSERT(*ptr == 20);
        }
    }

    {  // const vs non-const iteration
        auto nums = std::vector<int>{1, 2, 3};
        auto gen = zzz::enumerate(nums);

        // Use const iterator
        auto cit = gen.cbegin();
        ASSERT((std::pair{0, 1} == *cit));

        // Verify modification is not possible with const iterator
        static_assert(std::is_const_v<std::remove_reference_t<decltype(*cit)>>);
    }

    {  // Range-based for loop
        auto const words = std::vector<std::string>{"foo", "bar", "baz"};

        auto count = std::size_t{0};
        for (auto&& [i, word] : zzz::enumerate(words)) {
            ASSERT(i == count);
            if (i == 0) { ASSERT(word == "foo"); }
            if (i == 1) { ASSERT(word == "bar"); }
            if (i == 2) { ASSERT(word == "baz"); }
            count++;
        }
        ASSERT(count == 3);
    }

    {  // Modify values in-place
        auto words = std::vector{
            std::string{"foo"},
            std::string{"bar"},
            std::string{"baz"},
        };

        for (auto&& [i, word] : zzz::enumerate(words)) {
            word = "hello";
        }

        ASSERT(words[0] == "hello");
        ASSERT(words[1] == "hello");
        ASSERT(words[2] == "hello");
    }
}

TEST(zip)
{
    {  // Two containers
        auto const x = std::array{1, 2, 3, 4, 5};
        auto const y = std::array{"abc", "def", "ghi", "jkl", "mno"};

        auto i = 0;
        for (auto&& [a, b] : zzz::zip(x, y)) {
            ASSERT(a == x[i]);
            ASSERT(b == y[i]);
            ++i;
        }
        ASSERT(i == 5);
    }

    {  // Three containers
        auto const x = std::vector<int>{1, 2, 3, 4, 5};
        auto const y = std::vector<std::string>{"abc", "def", "ghi", "jkl", "mno"};
        auto const z = std::vector{4.5, 3.234, 2.5, 52.6, 32.34};

        auto i = 0;
        for (auto&& [a, b, c] : zzz::zip(x, y, z)) {
            ASSERT(a == x[i]);
            ASSERT(b == y[i]);
            ASSERT(c == z[i]);
            ++i;
        }
        ASSERT(i == 5);
    }

    {  // Modify values in-place
        auto x = std::vector<int>{1, 2, 3};
        auto y = std::vector<int>{4, 5, 6};

        for (auto&& [a, b] : zzz::zip(x, y)) {
            a = 10;
            b = 20;
        }

        ASSERT(x[0] == 10);
        ASSERT(x[1] == 10);
        ASSERT(x[2] == 10);
        ASSERT(y[0] == 20);
        ASSERT(y[1] == 20);
        ASSERT(y[2] == 20);
    }
}

TEST(iota)
{
    {  // Positive step
        auto gen = zzz::iota(0, 5);
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

    {  // Negative step
        auto gen = zzz::iota(5, 0, -1);
        auto it = gen.begin();
        ASSERT(it != gen.end());
        ASSERT(*it == 5);
        ++it;
        ASSERT(it != gen.end());
        ASSERT(*it == 4);
        ++it;
        ASSERT(it != gen.end());
        ASSERT(*it == 3);
        ++it;
        ASSERT(it != gen.end());
        ASSERT(*it == 2);
        ++it;
        ASSERT(it != gen.end());
        ASSERT(*it == 1);
        ++it;
        ASSERT(it == gen.end());
    }

    {  // Zero step
        auto gen = zzz::iota(0, 5, 0);
        ASSERT(gen.begin() == gen.end());
    }

    {  // Empty range
        auto gen = zzz::iota(5, 5);
        ASSERT(gen.begin() == gen.end());
    }
}

TEST(filter)
{
    {  // Even numbers
        auto const x = std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto const evens = zzz::filter(x, [](int i) { return i % 2 == 0; });

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

    {  // No elements
        auto const x = std::vector<int>{};
        auto const evens = zzz::filter(x, [](int i) { return i % 2 == 0; });
        ASSERT(evens.begin() == evens.end());
    }

    {  // All elements
        auto const x = std::vector{1, 3, 5, 7, 9};
        auto const odds = zzz::filter(x, [](int i) { return i % 2 != 0; });
        ASSERT(odds.begin() != odds.end());
    }

    {  // Modify values in-place
        auto x = std::vector<int>{1, 2, 3, 4, 5};
        for (auto&& i : zzz::filter(x, [](int i) { return i % 2 == 0; })) {
            i = 10;
        }
        ASSERT(x[0] == 1);
        ASSERT(x[1] == 10);
        ASSERT(x[2] == 3);
        ASSERT(x[3] == 10);
        ASSERT(x[4] == 5);
    }
}

TEST(map)
{
    {  // String lengths
        auto const x = std::vector<std::string>{"foo", "bark", "blaze"};
        auto const lengths = zzz::map(x, [](std::string const& s) { return s.size(); });

        auto it = lengths.begin();
        ASSERT(it != lengths.end());
        ASSERT(*it == 3);
        ++it;
        ASSERT(it != lengths.end());
        ASSERT(*it == 4);
        ++it;
        ASSERT(it != lengths.end());
        ASSERT(*it == 5);
        ++it;
        ASSERT(it == lengths.end());
    }

    {  // Multiply by 2
        auto const x = std::vector{1, 2, 3, 4, 5};
        auto const doubled = zzz::map(x, [](int i) { return i * 2; });

        auto it = doubled.begin();
        ASSERT(it != doubled.end());
        ASSERT(*it == 2);
        ++it;
        ASSERT(it != doubled.end());
        ASSERT(*it == 4);
        ++it;
        ASSERT(it != doubled.end());
        ASSERT(*it == 6);
        ++it;
        ASSERT(it != doubled.end());
        ASSERT(*it == 8);
        ++it;
        ASSERT(it != doubled.end());
        ASSERT(*it == 10);
        ++it;
        ASSERT(it == doubled.end());
    }

    {  // No elements
        auto const x = std::vector<int>{};
        auto const doubled = zzz::map(x, [](int i) { return i * 2; });
        ASSERT(doubled.begin() == doubled.end());
    }
}

TEST(take)
{
    {  // Take first 3 elements
        auto const x = std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto const first_three = zzz::take(x, 3);

        auto it = first_three.begin();
        ASSERT(it != first_three.end());
        ASSERT(*it == 1);
        ++it;
        ASSERT(it != first_three.end());
        ASSERT(*it == 2);
        ++it;
        ASSERT(it != first_three.end());
        ASSERT(*it == 3);
        ++it;
        ASSERT(it == first_three.end());
    }

    {  // Take all elements
        auto const x = std::vector{1, 2, 3, 4, 5};
        auto const all = zzz::take(x, 50);

        auto it = all.begin();
        ASSERT(it != all.end());
        ASSERT(*it == 1);
        ++it;
        ASSERT(it != all.end());
        ASSERT(*it == 2);
        ++it;
        ASSERT(it != all.end());
        ASSERT(*it == 3);
        ++it;
        ASSERT(it != all.end());
        ASSERT(*it == 4);
        ++it;
        ASSERT(it != all.end());
        ASSERT(*it == 5);
        ++it;
        ASSERT(it == all.end());
    }

    {  // Take no elements
        auto const x = std::vector<int>{};
        auto const none = zzz::take(x, 0);
        ASSERT(none.begin() == none.end());
    }

    {  // Modify values in-place
        auto x = std::vector<int>{1, 2, 3, 4, 5};
        for (auto& i : zzz::take(x, 3)) {
            i = 10;
        }
        ASSERT(x[0] == 10);
        ASSERT(x[1] == 10);
        ASSERT(x[2] == 10);
        ASSERT(x[3] == 4);
        ASSERT(x[4] == 5);
    }
}

TEST(take_while)
{
    {  // Take while less than 5
        auto const x = std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto const less_than_five = zzz::take_while(x, [](int i) { return i < 5; });

        auto it = less_than_five.begin();
        ASSERT(it != less_than_five.end());
        ASSERT(*it == 1);
        ++it;
        ASSERT(it != less_than_five.end());
        ASSERT(*it == 2);
        ++it;
        ASSERT(it != less_than_five.end());
        ASSERT(*it == 3);
        ++it;
        ASSERT(it != less_than_five.end());
        ASSERT(*it == 4);
        ++it;
        ASSERT(it == less_than_five.end());
    }

    {  // Take while always true
        auto const x = std::vector{1, 2, 3, 4, 5};
        auto const all = zzz::take_while(x, [](int) { return true; });

        auto it = all.begin();
        ASSERT(it != all.end());
        ASSERT(*it == 1);
        ++it;
        ASSERT(it != all.end());
        ASSERT(*it == 2);
        ++it;
        ASSERT(it != all.end());
        ASSERT(*it == 3);
        ++it;
        ASSERT(it != all.end());
        ASSERT(*it == 4);
        ++it;
        ASSERT(it != all.end());
        ASSERT(*it == 5);
        ++it;
        ASSERT(it == all.end());
    }

    {  // Take while always false
        auto const x = std::vector{1, 2, 3, 4, 5};
        auto const none = zzz::take_while(x, [](int) { return false; });
        ASSERT(none.begin() == none.end());
    }

    {  // Modify values in-place
        auto x = std::vector<int>{1, 2, 3, 4, 5};
        for (auto& i : zzz::take_while(x, [](int i) { return i < 3; })) {
            i = 10;
        }
        ASSERT(x[0] == 10);
        ASSERT(x[1] == 10);
        ASSERT(x[2] == 3);
        ASSERT(x[3] == 4);
        ASSERT(x[4] == 5);
    }
}

TEST(drop)
{
    {  // Drop first 3 elements
        auto const x = std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto const dropped = zzz::drop(x, 3);

        auto it = dropped.begin();
        ASSERT(it != dropped.end());
        ASSERT(*it == 4);
        ++it;
        ASSERT(it != dropped.end());
        ASSERT(*it == 5);
        ++it;
        ASSERT(it != dropped.end());
        ASSERT(*it == 6);
        ++it;
        ASSERT(it != dropped.end());
        ASSERT(*it == 7);
        ++it;
        ASSERT(it != dropped.end());
        ASSERT(*it == 8);
        ++it;
        ASSERT(it != dropped.end());
        ASSERT(*it == 9);
        ++it;
        ASSERT(it != dropped.end());
        ASSERT(*it == 10);
        ++it;
        ASSERT(it == dropped.end());
    }

    {  // Drop all elements
        auto const x = std::vector{1, 2, 3, 4, 5};
        auto const none = zzz::drop(x, 50);
        ASSERT(none.begin() == none.end());
    }

    {  // Drop no elements
        auto const x = std::vector<int>{1, 2};
        auto const all = zzz::drop(x, 0);
        auto it = all.begin();
        ASSERT(it != all.end());
        ASSERT(*it == 1);
        ++it;
        ASSERT(it != all.end());
        ASSERT(*it == 2);
        ++it;
        ASSERT(it == all.end());
    }

    {  // Modify values in-place
        auto x = std::vector<int>{1, 2, 3, 4, 5};
        for (auto& i : zzz::drop(x, 3)) {
            i = 10;
        }
        ASSERT(x[0] == 1);
        ASSERT(x[1] == 2);
        ASSERT(x[2] == 3);
        ASSERT(x[3] == 10);
        ASSERT(x[4] == 10);
    }
}

TEST(drop_while)
{
    {  // Drop while less than 5
        auto const x = std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto const dropped = zzz::drop_while(x, [](int i) { return i < 5; });

        auto it = dropped.begin();
        ASSERT(it != dropped.end());
        ASSERT(*it == 5);
        ++it;
        ASSERT(it != dropped.end());
        ASSERT(*it == 6);
        ++it;
        ASSERT(it != dropped.end());
        ASSERT(*it == 7);
        ++it;
        ASSERT(it != dropped.end());
        ASSERT(*it == 8);
        ++it;
        ASSERT(it != dropped.end());
        ASSERT(*it == 9);
        ++it;
        ASSERT(it != dropped.end());
        ASSERT(*it == 10);
        ++it;
        ASSERT(it == dropped.end());
    }

    {  // Drop while always true
        auto const x = std::vector{1, 2, 3, 4, 5};
        auto const none = zzz::drop_while(x, [](int) { return true; });
        ASSERT(none.begin() == none.end());
    }

    {  // Drop while always false
        auto const x = std::vector{1, 2, 3, 4, 5};
        auto const all = zzz::drop_while(x, [](int) { return false; });

        auto it = all.begin();
        ASSERT(it != all.end());
        ASSERT(*it == 1);
        ++it;
        ASSERT(it != all.end());
        ASSERT(*it == 2);
        ++it;
        ASSERT(it != all.end());
        ASSERT(*it == 3);
        ++it;
        ASSERT(it != all.end());
        ASSERT(*it == 4);
        ++it;
        ASSERT(it != all.end());
        ASSERT(*it == 5);
        ++it;
        ASSERT(it == all.end());
    }

    {  // Modify values in-place
        auto x = std::vector<int>{1, 2, 3, 4, 5};
        for (auto& i : zzz::drop_while(x, [](int i) { return i < 3; })) {
            i = 10;
        }
        ASSERT(x[0] == 1);
        ASSERT(x[1] == 2);
        ASSERT(x[2] == 10);
        ASSERT(x[3] == 10);
        ASSERT(x[4] == 10);
    }
}

TEST(repeat)
{
    {  // Repeat 3 times
        auto const repeated = zzz::repeat(10, 3);
        auto it = repeated.begin();
        ASSERT(it != repeated.end());
        ASSERT(*it == 10);
        ++it;
        ASSERT(it != repeated.end());
        ASSERT(*it == 10);
        ++it;
        ASSERT(it != repeated.end());
        ASSERT(*it == 10);
        ++it;
        ASSERT(it == repeated.end());
    }

    {  // Repeat 0 times
        auto const repeated = zzz::repeat(10, 0);
        ASSERT(repeated.begin() == repeated.end());
    }
}