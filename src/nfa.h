// Copyright 2018 Ilia Pozdnyakov
// This file is distributed under the MIT license.
// See the LICENSE.txt file for details.

#ifndef NFA_H_
#define NFA_H_

#include <algorithm>
#include <iostream>
#include <list>
#include <queue>
#include <string>
#include <unordered_set>
#include <utility>

namespace magex {

class NFA {
 public:
  struct State {};
  struct Transition {
    State *from;
    std::string label;
    State *to;
    Transition(State *f, const std::string &l, State *t);
    Transition(State *f, std::string &&l, State *t);
  };

  NFA(const NFA &) = delete;
  NFA(NFA &&) = default;
  explicit NFA(const std::string &s);
  explicit NFA(std::string &&s);
  NFA();

  NFA &operator=(const NFA &) = delete;
  NFA &operator=(NFA &&) = default;

  State &initial_state() { return states_.front(); }
  State &terminal_state() { return states_.back(); }
  const State &initial_state() const { return states_.front(); }
  const State &terminal_state() const { return states_.back(); }

  bool empty() const { return transitions_.empty(); }

  void Merge(NFA &&other);
  void Concatenate(NFA &&other);
  void Sum(NFA &&other);
  void IterateAtLeastOnce();
  void Iterate();

  template <class CharIt>
  bool Accepts(CharIt str_begin, CharIt str_end, const State *st) const;

  template <class CharIt>
  bool Accepts(CharIt str_begin, CharIt str_end) const {
    return Accepts(str_begin, str_end, &initial_state());
  }
  bool Accepts(const std::string &s) const {
    return Accepts(s.begin(), s.end());
  }

  friend std::ostream &operator<<(std::ostream &out, const NFA &nfa);

 protected:
  std::list<State> states_;
  std::list<Transition> transitions_;
};

template <class CharIt>
bool NFA::Accepts(CharIt str_begin, CharIt str_end, const State *st) const {
  std::unordered_set<const State *> det_state;
  std::queue<const State *> queue;

  queue.push(st);
  det_state.insert(st);
  while (!queue.empty()) {
    const State *front = queue.front();
    queue.pop();
    if (front == &terminal_state() && str_begin == str_end) return true;
    for (const auto &trans : transitions_) {
      if (trans.from == front && trans.label.empty()) {
        if (det_state.count(trans.to) == 0) {
          queue.push(trans.to);
          det_state.insert(trans.to);
        }
      }
    }
  }

  for (const auto &trans : transitions_) {
    const std::string &l = trans.label;
    if (det_state.count(trans.from) != 0 && !l.empty()) {
      auto mismatch = std::mismatch(l.begin(), l.end(), str_begin, str_end);
      if (mismatch.first == l.end()) {
        if (Accepts(mismatch.second, str_end, trans.to)) return true;
      }
    }
  }

  return false;
}

}  // namespace magex

#endif  // NFA_H_
