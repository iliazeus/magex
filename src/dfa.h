// Copyright 2018 Ilia Pozdnyakov
// This file is distributed under the MIT license.
// See the LICENSE.txt file for details.

#ifndef DFA_H_
#define DFA_H_

#include <array>
#include <climits>
#include <iostream>
#include <string>
#include <vector>

namespace magex {

class DFA {
 public:
  DFA() = default;
  DFA(const DFA &) = default;
  DFA(DFA &&) = default;

  DFA &operator=(const DFA &) = default;
  DFA &operator=(DFA &&) = default;

  size_t initial_state() const { return 0; }
  bool is_terminal_state(size_t state) const { return table_[state][0] == 0; }

  size_t no_transition() const { return table_.max_size(); }

  template <class CharIt>
  bool Accepts(CharIt begin, CharIt end);

  bool Accepts(const std::string &str) {
    return Accepts(str.begin(), str.end());
  }

  friend std::ostream &operator<<(std::ostream &, const DFA &);

  std::vector<std::array<size_t, 1 << CHAR_BIT>> &table_for_testing() {
    return table_;
  }

 private:
  std::vector<std::array<size_t, 1 << CHAR_BIT>> table_;
};

template <class CharIt>
bool DFA::Accepts(CharIt begin, CharIt end) {
  size_t cur_state = initial_state();
  auto it = begin;
  for ( ; it != end; ++it) {
    cur_state = table_[cur_state][(unsigned char)*it];
    if (cur_state >= table_.size()) break;
  }
  return it == end && is_terminal_state(cur_state);
}

}  // namespace magex

#endif  // DFA_H_