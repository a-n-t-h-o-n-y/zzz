#include <zzz/aggregate_magic.hpp>
#include <zzz/test.hpp>

TEST(aggregate_magic_general)
{
    {  // Copies
        struct {
            int a;
            double b;
            char c;
        } x{1, 2., 'c'};

        auto t = zzz::to_tuple(x);
        ASSERT(std::get<0>(t) == 1);
        std::get<0>(t) = 2;
        ASSERT(std::get<0>(t) == 2);
        ASSERT(x.a == 1);

        ASSERT(std::get<1>(t) == 2.);
        ASSERT(std::get<2>(t) == 'c');
    }

    {  // References
        struct {
            int a;
            double b;
            char c;
        } x{1, 2., 'c'};

        auto t = zzz::to_ref_tuple(x);
        ASSERT(std::get<0>(t) == 1);
        std::get<0>(t) = 2;
        ASSERT(std::get<0>(t) == 2);
        ASSERT(x.a == 2);
        ASSERT(std::get<1>(t) == 2.);
        ASSERT(std::get<2>(t) == 'c');
    }
}
