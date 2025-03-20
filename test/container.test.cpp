#include <array>
#include <functional>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include <zzz/container.hpp>
#include <zzz/test.hpp>

TEST(head_tail)
{
    {
        auto const x = std::vector<int>{1, 2, 3, 4, 5};
        ASSERT(*zzz::head(x) == 1);
        ASSERT(*zzz::tail(x) == 5);
    }
    {
        auto const x = std::vector<int>{};
        ASSERT(!zzz::head(x).has_value());
        ASSERT(!zzz::tail(x).has_value());
    }
    {
        auto const x = std::string{"foobar"};
        ASSERT(*zzz::head(x) == 'f');
        ASSERT(*zzz::tail(x) == 'r');
    }
    {
        auto const x = std::string{""};
        ASSERT(!zzz::head(x).has_value());
        ASSERT(!zzz::tail(x).has_value());
    }
}

TEST(lookup)
{
    {
        auto const x = std::map<int, std::string>{{1, "foo"}, {2, "bar"}, {4, "baz"}};
        ASSERT(*zzz::lookup(x, 2) == "bar");
        ASSERT(*zzz::lookup(x, 1) == "foo");
        ASSERT(*zzz::lookup(x, 4) == "baz");
        ASSERT(!zzz::lookup(x, 3).has_value());
        ASSERT(!zzz::lookup(x, 300).has_value());
    }
    {
        auto const x =
            std::unordered_map<int, std::string>{{1, "foo"}, {2, "bar"}, {4, "baz"}};
        ASSERT(*zzz::lookup(x, 2) == "bar");
        ASSERT(*zzz::lookup(x, 1) == "foo");
        ASSERT(*zzz::lookup(x, 4) == "baz");
        ASSERT(!zzz::lookup(x, 3).has_value());
        ASSERT(!zzz::lookup(x, 300).has_value());
    }
}

TEST(contains)
{
    {
        auto const x = std::vector<int>{1, 2, 3, 4, 5};

        ASSERT(zzz::contains(x, 1));
        ASSERT(zzz::contains(x, 2));
        ASSERT(!zzz::contains(x, 30));
    }
    {
        auto const x = std::vector<int>{};

        ASSERT(!zzz::contains(x, 1));
        ASSERT(!zzz::contains(x, 2));
    }
}

TEST(reduce)
{
    {
        auto const x = std::vector<int>{1, 2, 3, 4, 5};
        auto const r = zzz::reduce(std::plus{}, 0, x);
        ASSERT(r == 15);
    }
    {
        auto const x = std::vector<int>{};
        auto const r = zzz::reduce(std::plus{}, 0, x);
        ASSERT(r == 0);
    }
    {
        auto const x = std::vector<int>{1, 2, 3, 4, 5};
        auto const sum = zzz::reduce(std::plus{});
        ASSERT(sum(0, x) == 15);
    }
    {
        auto const x = std::vector<int>{1, 2, 3, 4, 5};
        auto const sum = zzz::reduce(std::plus{}, 0);
        ASSERT(sum(x) == 15);
    }
    {
        auto const x = std::array<int, 5>{1, 2, 3, 4, 5};
        auto const r = zzz::reduce(std::multiplies{}, 1, x);
        ASSERT(r == 120);
    }
    {
        auto const x = std::array<int, 0>{};
        auto const r = zzz::reduce(std::multiplies{}, 1, x);
        ASSERT(r == 1);
    }
    {
        auto const x = std::array<int, 5>{1, 2, 3, 4, 5};
        auto const times = zzz::reduce(std::multiplies{});
        ASSERT(times(1, x) == 120);
    }
    {
        auto const x = std::array<int, 5>{1, 2, 3, 4, 5};
        auto const times = zzz::reduce(std::multiplies{}, 1);
        ASSERT(times(x) == 120);
    }
    {
        auto const x = std::string{"Hello, World!"};
        auto const length =
            zzz::reduce([](int total, char) { return total + 1; }, 0, x);
        ASSERT(length == 13);
    }
    {
        auto const x = std::string{""};
        auto const length =
            zzz::reduce([](int total, char) { return total + 1; }, 0, x);
        ASSERT(length == 0);
    }
    {
        auto const x = std::string{"Hello, World!"};
        auto const length = zzz::reduce([](int total, char) { return total + 1; });
        ASSERT(length(0, x) == 13);
    }
    {
        auto const x = std::string{"Hello, World!"};
        auto const length = zzz::reduce([](int total, char) { return total + 1; }, 0);
        ASSERT(length(x) == 13);
    }
}