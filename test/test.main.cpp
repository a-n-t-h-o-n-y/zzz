#include <array>
#include <functional>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <variant>
#include <vector>

#include <zzz/zzz.hpp>
#define TEST_MAIN
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
        auto const x = std::vector<int>{1, 2, 3, 4, 5};
        auto const check = zzz::contains(x);

        ASSERT(check(1));
        ASSERT(check(2));
        ASSERT(!check(30));
    }
    {
        auto const x = std::vector<int>{};

        ASSERT(!zzz::contains(x, 1));
        ASSERT(!zzz::contains(x, 2));
    }
    {
        auto const x = std::string{"foobarbaz"};

        ASSERT(zzz::contains(x, 'f'));
        ASSERT(zzz::contains(x, 'a'));
        ASSERT(!zzz::contains(x, 'y'));
        ASSERT(!zzz::contains(x, 'F'));
    }
    {
        auto const x = std::string{"foobarbaz"};
        auto const check = zzz::contains(x);

        ASSERT(check('f'));
        ASSERT(check('a'));
        ASSERT(!check('y'));
        ASSERT(!check('F'));
    }
    {
        auto const x = std::string{""};

        ASSERT(!zzz::contains(x, 'f'));
        ASSERT(!zzz::contains(x, 'a'));
    }
    {
        auto const x = std::string{"foobarbaz"};

        ASSERT(zzz::contains(x, "foo"));
        ASSERT(zzz::contains(x, "obar"));
        ASSERT(!zzz::contains(x, "aaa"));
        ASSERT(!zzz::contains(x, "asdflkjasdklfjlaskdjflkasjdflkjaksdjf"));
    }
    {
        auto const x = std::string{""};

        ASSERT(!zzz::contains(x, ""));
        ASSERT(!zzz::contains(x, "foo"));
        ASSERT(!zzz::contains(x, "asdflkasjdflkjasldkfjalsdkjf"));
    }
    {
        auto const x = std::string{"foobarbaz"};
        auto const check = zzz::contains(x);

        ASSERT(check("foo"));
        ASSERT(check("obar"));
        ASSERT(!check("aaa"));
        ASSERT(!check("asdflkjasdklfjlaskdjflkasjdflkjaksdjf"));
    }
}

TEST(zip)
{
    using namespace std::literals;
    {
        auto const x = std::vector{1, 2, 3, 4, 5};
        auto const y = std::vector<std::string>{"abc", "def", "ghi", "jkl", "mno"};

        {
            auto const z = zzz::zip(x, y);
            ASSERT(z.size() == x.size());
            ASSERT(std::get<0>(z[0]) == 1);
            ASSERT(std::get<1>(z[0]) == "abc");
            ASSERT(std::get<0>(z[1]) == 2);
            ASSERT(std::get<1>(z[1]) == "def");
            ASSERT(std::get<0>(z[4]) == 5);
            ASSERT(std::get<1>(z[4]) == "mno");
        }

        try {
            // zipping different lengths will throw.
            auto const z = std::vector<char>{'a', 'b', 'c'};
            auto const r = zzz::zip(x, z);
            ASSERT(false);
        }
        catch (std::runtime_error const&) {
            ASSERT(true);
        }
        catch (...) {
            ASSERT(false);
        }
    }
    {
        auto const x = std::vector{1, 2, 3, 4, 5};
        auto const y = std::vector<std::string>{"abc", "def", "ghi", "jkl", "mno"};
        auto const z = std::vector{4.5, 3.234, 2.5, 52.6, 32.34};

        auto const r = zzz::zip(x, y, z);
        ASSERT(r.size() == x.size());
        ASSERT(std::get<0>(r[0]) == 1);
        ASSERT(std::get<1>(r[0]) == "abc");
        ASSERT(std::get<2>(r[0]) == 4.5);
        ASSERT(std::get<0>(r[1]) == 2);
        ASSERT(std::get<1>(r[1]) == "def");
        ASSERT(std::get<2>(r[1]) == 3.234);
        ASSERT(std::get<0>(r[4]) == 5);
        ASSERT(std::get<1>(r[4]) == "mno");
        ASSERT(std::get<2>(r[4]) == 32.34);
    }
    {
        auto const x = std::array{1, 2, 3, 4, 5};
        auto const y = std::array{"abc", "def", "ghi", "jkl", "mno"};

        auto const z = zzz::zip(x, y);
        ASSERT(z.size() == x.size());
        ASSERT(std::get<0>(z[0]) == 1);
        ASSERT(std::get<1>(z[0]) == "abc"sv);
        ASSERT(std::get<0>(z[1]) == 2);
        ASSERT(std::get<1>(z[1]) == "def"sv);
        ASSERT(std::get<0>(z[4]) == 5);
        ASSERT(std::get<1>(z[4]) == "mno"sv);
    }
    {
        auto const x = std::array{1, 2, 3, 4, 5};
        auto const y = std::array{"abc", "def", "ghi", "jkl", "mno"};
        auto const z = std::array{4.5, 3.234, 2.5, 52.6, 32.34};

        auto const r = zzz::zip(x, y, z);
        ASSERT(r.size() == x.size());
        ASSERT(std::get<0>(r[0]) == 1);
        ASSERT(std::get<1>(r[0]) == "abc"sv);
        ASSERT(std::get<2>(r[0]) == 4.5);
        ASSERT(std::get<0>(r[1]) == 2);
        ASSERT(std::get<1>(r[1]) == "def"sv);
        ASSERT(std::get<2>(r[1]) == 3.234);
        ASSERT(std::get<0>(r[4]) == 5);
        ASSERT(std::get<1>(r[4]) == "mno"sv);
        ASSERT(std::get<2>(r[4]) == 32.34);
    }
}

TEST(split)
{
    {
        auto const x = zzz::split("foo bar baz");
        ASSERT(x.size() == 3);
        ASSERT(x[0] == "foo");
        ASSERT(x[1] == "bar");
        ASSERT(x[2] == "baz");
    }
    {
        auto const x = zzz::split("foo,bar,baz", ",");
        ASSERT(x.size() == 3);
        ASSERT(x[0] == "foo");
        ASSERT(x[1] == "bar");
        ASSERT(x[2] == "baz");
    }
    {
        auto const x = zzz::split("foo, bar, baz", ", ");
        ASSERT(x.size() == 3);
        ASSERT(x[0] == "foo");
        ASSERT(x[1] == "bar");
        ASSERT(x[2] == "baz");
    }
    {
        auto const x = zzz::split("foobarbaz");
        ASSERT(x.size() == 1);
        ASSERT(x[0] == "foobarbaz");
    }
    {
        auto const x = zzz::split("foo:::::bar:::baz", ":");
        ASSERT(x.size() == 9);
        ASSERT(x[0] == "foo");
        ASSERT(x[5] == "bar");
        ASSERT(x[8] == "baz");
    }
    {
        auto const x = zzz::split("", ",");
        ASSERT(zzz::is_empty(x));
    }
    {
        try {
            (void)zzz::split("foo,bar,baz", "");
            ASSERT(false);
        }
        catch (std::runtime_error const&) {
            ASSERT(true);
        }
        catch (...) {
            ASSERT(false);
        }
    }
}

TEST(substring) { ASSERT(zzz::substring("foobar", 3, 3) == "bar"); }

TEST(getline)
{
    auto is = std::istringstream{"Hello\nWorld\n"};
    auto const hello = zzz::getline(is);
    auto const world = zzz::getline(is);
    auto const empty = zzz::getline(is);
    auto const empty2 = zzz::getline(is);

    ASSERT(hello.has_value());
    ASSERT(*hello == "Hello");

    ASSERT(world.has_value());
    ASSERT(*world == "World");

    ASSERT(!empty.has_value());
    ASSERT(!empty2.has_value());

    // while (auto const line = zzz::getline(std::cin))
    //     std::cout << *line << '\n';
}

TEST(map)
{
    {
        auto const x = std::vector<int>{1, 2, 3, 4, 5};
        auto const r = zzz::map([](int i) { return i * 2; }, x);
        ASSERT(r.size() == x.size());
        ASSERT(r[0] == 2);
        ASSERT(r[1] == 4);
        ASSERT(r[2] == 6);
        ASSERT(r[3] == 8);
        ASSERT(r[4] == 10);
    }
    {
        auto const x = std::vector<int>{};
        auto const r = zzz::map([](int i) { return i + 1; }, x);
        ASSERT(zzz::is_empty(r));
    }
    {
        auto const x = std::array<int, 5>{1, 2, 3, 4, 5};
        auto const r = zzz::map([](int i) { return i * 2; }, x);
        ASSERT(r.size() == x.size());
        ASSERT(r[0] == 2);
        ASSERT(r[1] == 4);
        ASSERT(r[2] == 6);
        ASSERT(r[3] == 8);
        ASSERT(r[4] == 10);
    }
    {
        auto const x = std::array<int, 0>{};
        auto const r = zzz::map([](int i) { return i + 1; }, x);
        ASSERT(zzz::is_empty(r));
    }
    {
        auto const x = std::string{"abc"};
        auto const r = zzz::map([](int i) { return i + 1; }, x);
        ASSERT(r.size() == x.size());
        ASSERT(r[0] == 'b');
        ASSERT(r[1] == 'c');
        ASSERT(r[2] == 'd');
    }
    {
        auto const x = std::string{""};
        auto const r = zzz::map([](int i) { return i + 1; }, x);
        ASSERT(zzz::is_empty(r));
    }
    {
        auto const x = std::optional<int>{5};
        auto const r = zzz::map([](int i) { return i * 2; }, x);
        ASSERT(r.has_value());
        ASSERT(*r == 10);
    }
    {
        auto const x = std::optional<int>{std::nullopt};
        auto const r = zzz::map([](int i) { return i * 2; }, x);
        ASSERT(!r.has_value());
    }
    {
        struct Visitor {
            auto operator()(int) { return 4; }
            auto operator()(char) { return 5; }
            auto operator()(double) { return 6; }
        };

        ASSERT(zzz::map(Visitor{}, std::variant<int, char, double>{3.2}) == 6);
        ASSERT(zzz::map(Visitor{}, std::variant<int, char, double>{1}) == 4);
        ASSERT(zzz::map(Visitor{}, std::variant<int, char, double>{'g'}) == 5);
        ASSERT(zzz::map(Visitor{}, std::variant<char>{'g'}) == 5);
    }
}

TEST(uppercase_lowercase)
{
    {
        auto x = std::string{"fooBar123"};
        ASSERT(zzz::uppercase(x) == "FOOBAR123");

        auto y = std::string{""};
        ASSERT(zzz::uppercase(y) == "");
    }
    {
        auto x = std::string{"FOoBAR123"};
        ASSERT(zzz::lowercase(x) == "foobar123");

        auto y = std::string{""};
        ASSERT(zzz::lowercase(y) == "");
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

TEST(print)
{
    {
        auto ss = std::ostringstream{};
        auto const x = std::vector<int>{1, 2, 3, 4, 5};
        zzz::print(ss, x);
        ASSERT(ss.str() == "{ 1, 2, 3, 4, 5 }");
    }
    {
        auto ss = std::ostringstream{};
        auto const x = std::array<char, 5>{'a', 'b', 'c', 'd', 'e'};
        zzz::print(ss, x);
        ASSERT(ss.str() == "{ a, b, c, d, e }");
    }
}

TEST(char_traits)
{
    ASSERT(zzz::is_whitespace(' '));
    ASSERT(zzz::is_whitespace('\n'));
    ASSERT(zzz::is_whitespace('\t'));
    ASSERT(!zzz::is_whitespace('a'));
    ASSERT(!zzz::is_whitespace('0'));
    ASSERT(!zzz::is_whitespace('?'));

    ASSERT(zzz::is_alphabetic('a'));
    ASSERT(zzz::is_alphabetic('A'));
    ASSERT(!zzz::is_alphabetic('?'));
}