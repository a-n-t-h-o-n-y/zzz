#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "zzz/zzz.hpp"

void test_head_tail()
{
    {
        auto const x = std::vector<int>{1, 2, 3, 4, 5};

        assert(*zzz::head(x) == 1);
        assert(*zzz::tail(x) == 5);
    }
    {
        auto const x = std::vector<int>{};

        assert(!zzz::head(x).has_value());
        assert(!zzz::tail(x).has_value());
    }
    {
        auto const x = std::string{"foobar"};

        assert(*zzz::head(x) == 'f');
        assert(*zzz::tail(x) == 'r');
    }
    {
        auto const x = std::string{""};

        assert(!zzz::head(x).has_value());
        assert(!zzz::tail(x).has_value());
    }
}

void test_lookup()
{
    {
        auto const x =
            std::map<int, std::string>{{1, "foo"}, {2, "bar"}, {4, "baz"}};
        assert(*zzz::lookup(x, 2) == "bar");
        assert(*zzz::lookup(x, 1) == "foo");
        assert(*zzz::lookup(x, 4) == "baz");
        assert(!zzz::lookup(x, 3).has_value());
        assert(!zzz::lookup(x, 300).has_value());
    }
    {
        auto const x = std::unordered_map<int, std::string>{
            {1, "foo"}, {2, "bar"}, {4, "baz"}};
        assert(*zzz::lookup(x, 2) == "bar");
        assert(*zzz::lookup(x, 1) == "foo");
        assert(*zzz::lookup(x, 4) == "baz");
        assert(!zzz::lookup(x, 3).has_value());
        assert(!zzz::lookup(x, 300).has_value());
    }
}

void test_contains()
{
    {
        auto const x = std::vector<int>{1, 2, 3, 4, 5};

        assert(zzz::contains(x, 1));
        assert(zzz::contains(x, 2));
        assert(!zzz::contains(x, 30));
    }
    {
        auto const x     = std::vector<int>{1, 2, 3, 4, 5};
        auto const check = zzz::contains(x);

        assert(check(1));
        assert(check(2));
        assert(!check(30));
    }
    {
        auto const x = std::vector<int>{};

        assert(!zzz::contains(x, 1));
        assert(!zzz::contains(x, 2));
    }
    {
        auto const x = std::string{"foobarbaz"};

        assert(zzz::contains(x, 'f'));
        assert(zzz::contains(x, 'a'));
        assert(!zzz::contains(x, 'y'));
        assert(!zzz::contains(x, 'F'));
    }
    {
        auto const x     = std::string{"foobarbaz"};
        auto const check = zzz::contains(x);

        assert(check('f'));
        assert(check('a'));
        assert(!check('y'));
        assert(!check('F'));
    }
    {
        auto const x = std::string{""};

        assert(!zzz::contains(x, 'f'));
        assert(!zzz::contains(x, 'a'));
    }
    {
        auto const x = std::string{"foobarbaz"};

        assert(zzz::contains(x, "foo"));
        assert(zzz::contains(x, "obar"));
        assert(!zzz::contains(x, "aaa"));
        assert(!zzz::contains(x, "asdflkjasdklfjlaskdjflkasjdflkjaksdjf"));
    }
    {
        auto const x = std::string{""};

        assert(!zzz::contains(x, ""));
        assert(!zzz::contains(x, "foo"));
        assert(!zzz::contains(x, "asdflkasjdflkjasldkfjalsdkjf"));
    }
    {
        auto const x     = std::string{"foobarbaz"};
        auto const check = zzz::contains(x);

        assert(check("foo"));
        assert(check("obar"));
        assert(!check("aaa"));
        assert(!check("asdflkjasdklfjlaskdjflkasjdflkjaksdjf"));
    }
}

void test_zip()
{
    {
        auto const x = std::vector{1, 2, 3, 4, 5};
        auto const y =
            std::vector<std::string>{"abc", "def", "ghi", "jkl", "mno"};

        for (auto const& [a, b] : zzz::zip(x, y))
            std::cout << a << ' ' << b << '\n';

        try {
            // zipping different lengths will throw.
            auto const z = std::vector<char>{'a', 'b', 'c'};
            auto const r = zzz::zip(x, z);
            assert(false);
        }
        catch (std::runtime_error const&) {
            assert(true);
        }
        catch (...) {
            assert(false);
        }
    }
    {
        auto const x = std::vector{1, 2, 3, 4, 5};
        auto const y =
            std::vector<std::string>{"abc", "def", "ghi", "jkl", "mno"};
        auto const z = std::vector{4.5, 3.234, 2.5, 52.6, 32.34};

        for (auto const& [a, b, c] : zzz::zip(x, y, z))
            std::cout << a << ' ' << b << ' ' << c << '\n';
    }
    {
        auto const x = std::array{1, 2, 3, 4, 5};
        auto const y = std::array{"abc", "def", "ghi", "jkl", "mno"};

        for (auto const& [a, b] : zzz::zip(x, y))
            std::cout << a << ' ' << b << '\n';
    }
    {
        auto const x = std::array{1, 2, 3, 4, 5};
        auto const y = std::array{"abc", "def", "ghi", "jkl", "mno"};
        auto const z = std::array{4.5, 3.234, 2.5, 52.6, 32.34};

        for (auto const& [a, b, c] : zzz::zip(x, y, z))
            std::cout << a << ' ' << b << ' ' << c << '\n';
    }
    {
        auto const y = std::vector{"abc", "def", "ghi", "jkl", "mno"};
        for (auto const& [index, element] : zzz::zip_with_index(y))
            std::cout << index << ' ' << element << '\n';

        for (auto const& [index, element] : zzz::zip_with_index(y, 100))
            std::cout << index << ' ' << element << '\n';

        for (auto const& [i, e] : zzz::zip_with_index(std::vector<int>{}))
            std::cout << i << ' ' << e << '\n';
    }
}

void test_split()
{
    {
        auto const x = zzz::split("foo bar baz");
        assert(x.size() == 3);
        assert(x[0] == "foo");
        assert(x[1] == "bar");
        assert(x[2] == "baz");
    }
    {
        auto const x = zzz::split("foo,bar,baz", ",");
        assert(x.size() == 3);
        assert(x[0] == "foo");
        assert(x[1] == "bar");
        assert(x[2] == "baz");
    }
    {
        auto const x = zzz::split("foo, bar, baz", ", ");
        assert(x.size() == 3);
        assert(x[0] == "foo");
        assert(x[1] == "bar");
        assert(x[2] == "baz");
    }
    {
        auto const x = zzz::split("foobarbaz");
        assert(x.size() == 1);
        assert(x[0] == "foobarbaz");
    }
    {
        auto const x = zzz::split("foo:::::bar:::baz", ":");
        assert(x.size() == 9);
        assert(x[0] == "foo");
        assert(x[5] == "bar");
        assert(x[8] == "baz");
    }
    {
        auto const x = zzz::split("", ",");
        assert(zzz::is_empty(x));
    }
    {
        try {
            (void)zzz::split("foo,bar,baz", "");
            assert(false);
        }
        catch (std::runtime_error const&) {
            assert(true);
        }
        catch (...) {
            assert(false);
        }
    }
}

void test_substring() { assert(zzz::substring("foobar", 3, 3) == "bar"); }

void test_getline()
{
    auto is           = std::istringstream{"Hello\nWorld\n"};
    auto const hello  = zzz::getline(is);
    auto const world  = zzz::getline(is);
    auto const empty  = zzz::getline(is);
    auto const empty2 = zzz::getline(is);

    assert(hello.has_value());
    assert(*hello == "Hello");

    assert(world.has_value());
    assert(*world == "World");

    assert(!empty.has_value());
    assert(!empty2.has_value());

    // while (auto const line = zzz::getline(std::cin))
    //     std::cout << *line << '\n';
    // std::cout << "exiting\n";
}

void test_map()
{
    {
        auto const x = std::vector<int>{1, 2, 3, 4, 5};
        auto const r = zzz::map([](int i) { return i * 2; }, x);
        assert(r.size() == x.size());
        assert(r[0] == 2);
        assert(r[1] == 4);
        assert(r[2] == 6);
        assert(r[3] == 8);
        assert(r[4] == 10);
    }
    {
        auto const x = std::vector<int>{};
        auto const r = zzz::map([](int i) { return i + 1; }, x);
        assert(zzz::is_empty(r));
    }
    {
        auto const x = std::array<int, 5>{1, 2, 3, 4, 5};
        auto const r = zzz::map([](int i) { return i * 2; }, x);
        assert(r.size() == x.size());
        assert(r[0] == 2);
        assert(r[1] == 4);
        assert(r[2] == 6);
        assert(r[3] == 8);
        assert(r[4] == 10);
    }
    {
        auto const x = std::array<int, 0>{};
        auto const r = zzz::map([](int i) { return i + 1; }, x);
        assert(zzz::is_empty(r));
    }
    {
        auto const x = std::string{"abc"};
        auto const r = zzz::map([](int i) { return i + 1; }, x);
        assert(r.size() == x.size());
        assert(r[0] == 'b');
        assert(r[1] == 'c');
        assert(r[2] == 'd');
    }
    {
        auto const x = std::string{""};
        auto const r = zzz::map([](int i) { return i + 1; }, x);
        assert(zzz::is_empty(r));
    }
    {
        auto const x = std::optional<int>{5};
        auto const r = zzz::map([](int i) { return i * 2; }, x);
        assert(r.has_value());
        assert(*r == 10);
    }
    {
        auto const x = std::optional<int>{std::nullopt};
        auto const r = zzz::map([](int i) { return i * 2; }, x);
        assert(!r.has_value());
    }
    {
        struct Visitor {
            auto operator()(int) { return 4; }
            auto operator()(char) { return 5; }
            auto operator()(double) { return 6; }
        };

        assert(zzz::map(Visitor{}, std::variant<int, char, double>{3.2}) == 6);
        assert(zzz::map(Visitor{}, std::variant<int, char, double>{1}) == 4);
        assert(zzz::map(Visitor{}, std::variant<int, char, double>{'g'}) == 5);
        assert(zzz::map(Visitor{}, std::variant<char>{'g'}) == 5);
    }
}

void test_upper_lowercase()
{
    {
        auto x = std::string{"fooBar123"};
        assert(zzz::uppercase(x) == "FOOBAR123");

        auto y = std::string{""};
        assert(zzz::uppercase(y) == "");
    }
    {
        auto x = std::string{"FOoBAR123"};
        assert(zzz::lowercase(x) == "foobar123");

        auto y = std::string{""};
        assert(zzz::lowercase(y) == "");
    }
}

void test_reduce()
{
    {
        auto const x = std::vector<int>{1, 2, 3, 4, 5};
        auto const r = zzz::reduce(std::plus{}, 0, x);
        assert(r == 15);
    }
    {
        auto const x = std::vector<int>{};
        auto const r = zzz::reduce(std::plus{}, 0, x);
        assert(r == 0);
    }
    {
        auto const x   = std::vector<int>{1, 2, 3, 4, 5};
        auto const sum = zzz::reduce(std::plus{});
        assert(sum(0, x) == 15);
    }
    {
        auto const x   = std::vector<int>{1, 2, 3, 4, 5};
        auto const sum = zzz::reduce(std::plus{}, 0);
        assert(sum(x) == 15);
    }
    {
        auto const x = std::array<int, 5>{1, 2, 3, 4, 5};
        auto const r = zzz::reduce(std::multiplies{}, 1, x);
        assert(r == 120);
    }
    {
        auto const x = std::array<int, 0>{};
        auto const r = zzz::reduce(std::multiplies{}, 1, x);
        assert(r == 1);
    }
    {
        auto const x     = std::array<int, 5>{1, 2, 3, 4, 5};
        auto const times = zzz::reduce(std::multiplies{});
        assert(times(1, x) == 120);
    }
    {
        auto const x     = std::array<int, 5>{1, 2, 3, 4, 5};
        auto const times = zzz::reduce(std::multiplies{}, 1);
        assert(times(x) == 120);
    }
    {
        auto const x = std::string{"Hello, World!"};
        auto const length =
            zzz::reduce([](int total, char) { return total + 1; }, 0, x);
        assert(length == 13);
    }
    {
        auto const x = std::string{""};
        auto const length =
            zzz::reduce([](int total, char) { return total + 1; }, 0, x);
        assert(length == 0);
    }
    {
        auto const x = std::string{"Hello, World!"};
        auto const length =
            zzz::reduce([](int total, char) { return total + 1; });
        assert(length(0, x) == 13);
    }
    {
        auto const x = std::string{"Hello, World!"};
        auto const length =
            zzz::reduce([](int total, char) { return total + 1; }, 0);
        assert(length(x) == 13);
    }
}

void test_print()
{
    {
        auto const x = std::vector<int>{1, 2, 3, 4, 5};
        zzz::print(std::cout, x);
        std::cout << '\n';
    }
    {
        auto const x = std::array<char, 5>{'a', 'b', 'c', 'd', 'e'};
        zzz::print(std::cout, x);
        std::cout << '\n';
    }
}

void test_char_traits()
{
    assert(zzz::is_whitespace(' '));
    assert(zzz::is_whitespace('\n'));
    assert(zzz::is_whitespace('\t'));
    assert(!zzz::is_whitespace('a'));
    assert(!zzz::is_whitespace('0'));
    assert(!zzz::is_whitespace('?'));

    assert(zzz::is_alphabetic('a'));
    assert(zzz::is_alphabetic('A'));
    assert(!zzz::is_alphabetic('?'));
}

int main()
{
    test_head_tail();
    test_lookup();
    test_contains();
    test_zip();
    test_split();
    test_substring();
    test_getline();
    test_map();
    test_upper_lowercase();
    test_reduce();
    test_print();
    test_char_traits();
    std::cout << "SUCCESS\n";
    return 0;
}
