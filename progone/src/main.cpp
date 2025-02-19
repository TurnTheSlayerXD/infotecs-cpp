#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <iostream>
#include <mutex>
#include <ranges>
#include <thread>

#include "Buffer.h"

#include "CharCount.h"

constexpr int alp_size = 52;

using data_type = std::vector<CharCount>;

data_type count_entries(const auto &str) {
  std::array<int, alp_size> din{};
  for (const char c : str) {
    int ind = c >= 'a' ? c - 'a' : c - 'A' + alp_size / 2;
    ++din[ind];
  }
  std::vector<CharCount> vec;
  for (int i = 0; i < alp_size / 2; ++i) {
    if (din[i] > 0) {
      vec.emplace_back(static_cast<char>('a' + i), din[i]);
    }
  }
  for (int i = alp_size / 2; i < alp_size; ++i) {
    if (din[i] > 0) {
      vec.emplace_back(static_cast<char>('A' + i - alp_size / 2), din[i]);
    }
  }
  return vec;
}

void thread_one(Buffer<data_type> &buf) {
  namespace rng = std::ranges;

  for (;;) {
    std::string input;
    std::cin >> input;

    auto is_letter = [](char c) {
      return std::isalpha(static_cast<unsigned char>(c));
    };
    if (input.size() > 64) {
      std::cerr << "Wrong input. Length of input sequence is greater than 64 "
                   "symbols\n";
      continue;
    }
    if (!rng::all_of(input, is_letter)) {
      std::cerr
          << "Wrong input. Input sequence contains non-alphabetic letters\n";
      continue;
    }

    auto entries = count_entries(input);
    buf.add(entries);
  }
}

void thread_two(Buffer<data_type> &buf) {
  std::mutex mut;
  for (;;) {
    auto &cv = buf.get_cv();
    std::unique_lock lk(mut);
    cv.wait(lk, [&buf] { return buf.size() > 0; });

    data_type data;
    if (buf.empty() || !buf.pop(data)) {
      continue;
    }

    for (const auto &d : data) {
      std::cout << d;
    }
    std::cout << "\n";
  }
}

int main(int argc, char *argv[]) {
  Buffer<data_type> buf;
  std::jthread one(thread_one, std::ref(buf));
  std::jthread two(thread_two, std::ref(buf));

  return 0;
}
