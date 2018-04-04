// Copyright 2018 Ilia Pozdnyakov
// This file is distributed under the MIT license.
// See the LICENSE.txt file for details.

#ifndef NFA_H_
#define NFA_H_

#include <list>
#include <string>
#include <utility>
#include <algorithm>
#include <iostream>
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
    Transition(State *f, const std::string &l, State *t)
      : from(f), label(l), to(t) {}
    Transition(State *f, std::string &&l, State *t)
      : from(f), label(std::move(l)), to(t) {}
  };

  NFA(const NFA &) = delete;
  NFA(NFA &&) = default;
  explicit NFA(const std::string &s)
    : states_(2)
    , transitions_{{&states_.front(), s, &states_.back()}} {}
  explicit NFA(std::string &&s)
    : states_(2)
    , transitions_{{&states_.front(), std::move(s), &states_.back()}} {}
  NFA(): states_(2) {}

  NFA &operator=(const NFA &) = delete;
  NFA &operator=(NFA &&) = default;

  State &initialState() { return states_.front(); }
  State &terminalState() { return states_.back(); }
  const State &initialState() const { return states_.front(); }
  const State &terminalState() const { return states_.back(); }

  bool empty() const { return transitions_.empty(); }

  void Merge(NFA &&other) {
    states_.splice(states_.end(), std::move(other.states_));
    transitions_.splice(transitions_.end(), std::move(other.transitions_));
  }

  void Concatenate(NFA &&other) {
    transitions_.emplace_back(&terminalState(), "", &other.initialState());
    Merge(std::move(other));
  }

  void Sum(NFA &&other) {
    State *thisInit = &initialState();
    State *otherInit = &other.initialState();
    State *thisTerm = &terminalState();
    State *otherTerm = &other.terminalState();

    states_.emplace_front();
    transitions_.emplace_front(&initialState(), "", thisInit);
    transitions_.emplace_front(&initialState(), "", otherInit);

    Merge(std::move(other));

    states_.emplace_back();
    transitions_.emplace_back(thisTerm, "", &terminalState());
    transitions_.emplace_back(otherTerm, "", &terminalState());
  }

  void IterateAtLeastOnce() {
    transitions_.emplace_back(&terminalState(), "", &initialState());
  }

  void Iterate() {
    transitions_.emplace_back(&terminalState(), "", &initialState());
    transitions_.emplace_back(&initialState(), "", &terminalState());
  }

  template<class CharIt>
  bool Accepts(CharIt strBegin, CharIt strEnd, const State *st) const {
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
  template<class CharIt>
  bool Accepts(CharIt strBegin, CharIt strEnd) const {
    return Accepts(strBegin, strEnd, &initialState());
  }
  bool Accepts(const std::string &s) const {
    return Accepts(s.begin(), s.end());
  }

  friend std::ostream &operator<<(std::ostream &out, const NFA &nfa) {
    for (const auto &trans : nfa.transitions_) {
      out << trans.from;
      out << " \"" << trans.label << "\" ";
      out << trans.to;
      out << std::endl;
    }
    return out;
  }

  std::list<State> states_;
  std::list<Transition> transitions_;
};

}  // namespace magex

#endif  // NFA_H_
