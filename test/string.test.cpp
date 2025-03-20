#include <zzz/string.hpp>
#include <zzz/test.hpp>

TEST(string_containes)
{
    {
        auto const x = std::string{"foobarbaz"};

        ASSERT(zzz::contains(x, 'f'));
        ASSERT(zzz::contains(x, 'a'));
        ASSERT(!zzz::contains(x, 'y'));
        ASSERT(!zzz::contains(x, 'F'));
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
        ASSERT(zzz::contains("foobarbaz", "foo"));
        ASSERT(zzz::contains("foobarbaz", "obar"));
        ASSERT(!zzz::contains("foobarbaz", "aaa"));
        ASSERT(!zzz::contains("foobarbaz", "asdflkjasdklfjlaskdjflkasjdflkjaksdjf"));
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