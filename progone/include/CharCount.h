
#ifndef CHARCOUNT_H
#define CHARCOUNT_H

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>

#include <charconv>
struct CharCount {
  char letter;
  int count;

  CharCount(char letter, int count) : letter(letter), count(count) {}

  CharCount(const CharCount &o) = default;
  CharCount(CharCount &&o) noexcept = default;

  CharCount &operator=(const CharCount &o) = default;
  CharCount &operator=(CharCount &&o) noexcept = default;

  template <class C>
  static std::pair<CharCount, bool> from_string(const C &str) {
    const auto false_return = std::make_pair(CharCount{' ', 0}, false);

    namespace rng = std::ranges;
    auto stick_pos = rng::find(str, '|');

    const auto end = str.end();

    if (stick_pos == end || rng::distance(str.begin(), stick_pos) != 1 ||
        !std::isalpha(static_cast<unsigned char>(str[0]))) {
      return false_return;
    }

    const char letter = str[0];

    const auto view = std::string_view(stick_pos + 1, end);

    int count;
    const auto [_, err] =
        std::from_chars(view.data(), view.data() + view.size(), count, 10);

    if (err != std::errc()) {
      return false_return;
    }

    return std::make_pair(CharCount{letter, count}, true);
  }
};

std::ostream &operator<<(std::ostream &str, const CharCount &d) {
  str << d.letter << "|" << d.count << " ";
  return str;
}

#endif