#pragma once
#include <cctype>

namespace zzz {

[[nodiscard]] inline auto is_whitespace(char c) -> bool { return std::isspace(c) != 0; }

[[nodiscard]] inline auto is_alphabetic(char c) -> bool { return std::isalpha(c) != 0; }

[[nodiscard]] inline auto is_digit(char c) -> bool { return std::isdigit(c) != 0; }

[[nodiscard]] inline auto is_hexidecimal_digit(char c) -> bool
{
    return std::isxdigit(c) != 0;
}

[[nodiscard]] inline auto is_control(char c) -> bool { return std::iscntrl(c) != 0; }

[[nodiscard]] inline auto is_graphical(char c) -> bool { return std::isgraph(c) != 0; }

[[nodiscard]] inline auto is_blank(char c) -> bool { return std::isblank(c) != 0; }

[[nodiscard]] inline auto is_alphanumeric(char c) -> bool
{
    return std::isalnum(c) != 0;
}

[[nodiscard]] inline auto is_lowercase(char c) -> bool { return std::islower(c) != 0; }

[[nodiscard]] inline auto is_uppercase(char c) -> bool { return std::isupper(c) != 0; }

[[nodiscard]] inline auto is_punctuation(char c) -> bool
{
    return std::ispunct(c) != 0;
}

[[nodiscard]] inline auto is_printable(char c) -> bool { return std::isprint(c) != 0; }

}  // namespace zzz