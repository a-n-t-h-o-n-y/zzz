#include <tuple>

#include <zzz/test.hpp>
#include <zzz/tuple.hpp>

TEST(for_each)
{
    {  // Sum uniform
        auto t = std::tuple{1, 2, 3, 4, 5};
        auto sum = 0;
        zzz::for_each(t, [&](auto x) { sum += x; });
        ASSERT(sum == 15);
    }

    {  // Modify
        auto t = std::tuple{1, 2, 3, 4, 5};
        zzz::for_each(t, [](auto& x) { x *= 2; });
        ASSERT((t == std::tuple{2, 4, 6, 8, 10}));
    }

    {  // Sum mixed
        auto t = std::tuple{1, 2.5, 3, 4.2, 5};
        auto sum = 0.0;
        zzz::for_each(t, [&](auto x) { sum += x; });
        ASSERT(sum == 15.7);
    }

    {  // Modify mixed
        auto t = std::tuple{1, 2.5, 3, 4.2, 5};
        zzz::for_each(t, [](auto& x) { x *= 2; });
        ASSERT((t == std::tuple{2, 5.0, 6, 8.4, 10}));
    }
}