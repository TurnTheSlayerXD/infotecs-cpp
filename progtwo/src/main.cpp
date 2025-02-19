#include "CharCount.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>

#include "ipc_interaction.h"

namespace rng = std::ranges;


int main() {

  for (;;) {
    const auto [input, is_succ] = ipc::receive_message();

    if (!is_succ) {
      continue;
    }

    std::vector<CharCount> vec;


    auto is_space = [](const char &c) { return c == ' '; };
    const auto end = input.end();

    for (auto cur = rng::find_if_not(input.begin(), end, is_space),
              next = rng::find_if(cur, end, is_space);
         cur != end; cur = rng::find_if_not(next, end, is_space),
              next = rng::find_if(cur, end, is_space)) {

      std::string_view view = {cur, next};

      const auto [obj, is_succ] = CharCount::from_string(view);

      if (!is_succ) {
        std::cerr << "Error while parsing substring: " << view << "\n";
      } else {
        vec.push_back(obj);
      }

    }

    rng::sort(vec, [](const CharCount &a, const CharCount &b) {
      return (a.count < b.count) || (a.count == b.count && a.letter < b.letter);
    });

    for (const auto &v : vec) {
      std::cout << v;
    }
    std::cout << "\n";
  }

  return 0;
}