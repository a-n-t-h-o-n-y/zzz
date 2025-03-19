#pragma once

#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace zzz::test {

/** Represents a test case.
 *  @details Holds a test function and its name.
 */
struct TestCase {
    std::string name;
    std::function<void()> test_func;
};

/** Returns a mutable list of registered tests.
 *  @return Reference to the static vector of tests.
 */
[[nodiscard]] inline auto get_test_cases() -> std::vector<TestCase>&
{
    static std::vector<TestCase> tests;
    return tests;
}

// Global counter for assertions executed in the current test.
inline thread_local int assert_count = 0;

/** Runs all registered tests.
 *  @return The number of failed tests.
 */
[[nodiscard]] inline auto run_tests() -> int
{
    // Terminal escape codes.
    constexpr auto RESET = "\033[0m";  // Reset to default color
    constexpr auto GREEN = "\033[32m";
    constexpr auto RED = "\033[31m";
    constexpr auto YELLOW = "\033[33m";
    constexpr auto BOLD = "\033[1m";

    int failures = 0;
    for (auto const& test : get_test_cases()) {
        try {
            assert_count = 0;  // Reset assertion counter for each test.
            test.test_func();
            std::cout << test.name << GREEN << BOLD << " passed." << RESET << " ("
                      << assert_count << " assertions)\n";
        }
        catch (std::exception const& ex) {
            std::cout << test.name << RED << BOLD << " failed: " << ex.what() << RESET
                      << " (" << assert_count << " assertions passed before failure)\n";
            ++failures;
        }
        catch (...) {
            std::cout << test.name << YELLOW << BOLD << " failed with unknown error."
                      << RESET << " (" << assert_count
                      << " assertions before failure)\n";
            ++failures;
        }
    }
    return failures;
}

}  // namespace zzz::test

/** Macro to register a test case.
 *  @details The macro declares the test function, registers it, and then
 * defines it.
 */
#define TEST(name)                                                               \
    static void test_##name();                                                   \
    struct test_##name##_registrar {                                             \
        test_##name##_registrar()                                                \
        {                                                                        \
            zzz::test::get_test_cases().push_back({"test_" #name, test_##name}); \
        }                                                                        \
    } test_##name##_registrar_instance;                                          \
    static void test_##name()

/** Macro for assertions.
 *  @details Throws a std::runtime_error if the expression is false.
 *  Increments the assertion counter if the expression is true.
 */
#define ASSERT(expr)                                                       \
    do {                                                                   \
        if (!(expr)) throw std::runtime_error("Assertion failed: " #expr); \
        ++zzz::test::assert_count;                                         \
    } while (0)

#ifdef TEST_MAIN

/** Main entry point to run tests.
 *  @return Zero if all tests pass, or the number of failures.
 */
[[nodiscard]] auto main() -> int { return zzz::test::run_tests(); }

#endif  // TEST_MAIN