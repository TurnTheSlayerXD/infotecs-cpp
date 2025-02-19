#include "CharCount.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>

#include "ipc_interaction.h"

namespace rng = std::ranges;

auto iter_input(const std::string &input) {
  auto is_space = [](const char &c) { return c == ' '; };
  const auto end = input.end();

  decltype(input.begin()) iter;
  decltype(iter) next;

  auto fuu = [&iter, &next, &end, &is_space]() {
    iter = rng::find_if_not(next, end, is_space);
    next = rng::find_if(iter, end, is_space);
    return std::make_pair(iter, next);
  };

  return fuu;
}

int main() {

  for (;;) {
    const auto [input, is_err] = ipc::receive_message();
    if (is_err) {
      continue;
    }

    std::vector<CharCount> vec;

    auto iter = iter_input(input);
    auto [cur, next] = iter();
    
    while (cur != input.end()) {

      std::string_view view = {cur, next};

      const auto [obj, is_succ] = CharCount::from_string(view);

      if (!is_succ) {
        std::cerr << "Error while parsing substring: " << view << "\n";
      } else {
        vec.push_back(obj);
      }

      const auto ret = iter();
      cur = std::get<0>(ret);
      next = std::get<1>(ret);
    }

    rng::sort(vec, [](const CharCount &a, const CharCount &b) {
      return a.count < b.count;
    });

    for (const auto &v : vec) {
      std::cout << v;
    }
    std::cout << "\n";
  }

  return 0;
}