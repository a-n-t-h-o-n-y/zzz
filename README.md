# 💤

Collection of C++17 Convenience Functions

Single Header `zzz/zzz.hpp`

`namespace zzz`

## Contents

- `is_empty(Container) -> bool`
- `head(Container<T>) -> optional<T>`
- `tail(Container<T>) -> optional<T>`
- `lookup(Mappable<K, V>, K) -> optional<V>`
- `find(string_view, char) -> string_view::const_iterator`
- `find(string_view, string_view segment) -> string_view::const_iterator`
- `find(vector<T>, T) -> vector<T>::const_iterator`
- `contains(vector<T>, T) -> bool`
- `contains(string_view, char) -> bool`
- `contains(string_view, string_view) -> bool`
- `contains(Container) -> partial application - lambda`
- `zip(vector<Ts>...) -> vector<tuple<Ts...>>`
- `zip(array<Ts, N>...) -> array<tuple<Ts...>, N>`
- `zip_with_index(vector<T>, size_t begin = 0) -> vector<pair<size_t, T>>`
- `split(string_view, string_view delim = " ") -> vector<string_view>`
- `substring(string_view, size_t begin, size_t length) -> string_view`
- `getline(istream&, char delim = '\n') -> optional<string>`
- `map(F, vector<T>) -> vector<invoke_result_t<F, T>>`
- `map(F, array<T, N>) -> array<invoke_result_t<F, T>, N>`
- `map(F, string_view) -> string`
- `map(F, optional<T>) -> optional<invoke_result_t<F, T>>`
- `map(F, variant<Ts...>) -> decltype(visit(F, variant<>))`
- `uppercase(string_view) -> string`
- `lowercase(string_view) -> string`
- `reduce(F, U init, vector<T>) -> U`
- `reduce(F, U init, array<T, N>) -> U`
- `reduce(F, U init, string_view) -> U`
- `reduce(F) -> partial application - lambda`
- `reduce(F, U init) -> partial application - lambda`
- `print(ostream&, vector<T>) -> ostream&`
- `print(ostream&, array<T, N>) -> ostream&`
- `is_whitespace(char) -> bool`
- `is_alphabetic(char) -> bool`
- `is_digit(char) -> bool`
- `is_alphanumeric(char) -> bool`
- `is_hexadecimal_digit(char) -> bool`
- `is_control(char) -> bool`
- `is_graphical(char) -> bool`
- `is_blank(char) -> bool`
- `is_lowercase(char) -> bool`
- `is_uppercase(char) -> bool`
- `is_puctuation(char) -> bool`
- `is_printable(char) -> bool`

## Build

`CMakeLists.txt` provides the `zzz` interface target.

## Tests

Basic tests with target `zzz-test`.
