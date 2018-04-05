// Copyright 2018 Ilia Pozdnyakov
// This file is distributed under the MIT license.
// See the LICENSE.txt file for details.

#ifndef NFA_H_
#define NFA_H_

#include <list>
#include <string>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <queue>

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
  NFA(): states_(2) {}

  NFA &operator=(const NFA &) = delete;
  NFA &operator=(NFA &&) = default;

  State &initialState() { return states_.front(); }
  State &terminalState() { return states_.back(); }
  const State &initialState() const { return states_.front(); }
  const State &terminalState() const { return states_.back(); }

  bool empty() const { return transitions_.empty(); }

  void Merge(NFA &&other);
  void Concatenate(NFA &&other);
  void Sum(NFA &&other);
  void IterateAtLeastOnce();
  void Iterate();

  template<class CharIt>
  bool Accepts(CharIt strBegin, CharIt strEnd, const State *st) const;

  template<class CharIt>
  bool Accepts(CharIt strBegin, CharIt strEnd) const {
    return Accepts(strBegin, strEnd, &initialState());
  }
  bool Accepts(const std::string &s) const {
    return Accepts(s.begin(), s.end());
  }

  friend std::ostream &operator<<(std::ostream &out, const NFA &nfa);

 protected:
  std::list<State> states_;
  std::list<Transition> transitions_;
};

template<class CharIt>
bool NFA::Accepts(CharIt strBegin, CharIt strEnd, const State *st) const {
  std::unordered_set<const State *> detState;
  std::queue<const State *> queue;

  queue.push(st);
  detState.insert(st);
  while (!queue.empty()) {
    const State *front = queue.front();
    queue.pop();
    if (front == &terminalState() && strBegin == strEnd) return true;
    for (const auto &trans : transitions_) {
      if (trans.from == front && trans.label.empty()) {
        if (detState.count(trans.to) == 0) {
          queue.push(trans.to);
          detState.insert(trans.to);
        }
      }
    }
  }

  for (const auto &trans : transitions_) {
    const std::string &l = trans.label;
    if (detState.count(trans.from) != 0 && !l.empty()) {
      auto mismatch = std::mismatch(l.begin(), l.end(), strBegin, strEnd);
      if (mismatch.first == l.end()) {
        if (Accepts(mismatch.second, strEnd, trans.to)) return true;
      }
    }
  }

  return false;
}

}  // namespace magex

#endif  // NFA_H_
