#include <array>
#include <sstream>
#include <vector>

#include <zzz/io.hpp>
#include <zzz/test.hpp>

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
