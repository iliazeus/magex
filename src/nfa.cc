// Copyright 2018 Ilia Pozdnyakov
// This file is distributed under the MIT license.
// See the LICENSE.txt file for details.

#include "./nfa.h"

namespace magex {

NFA::Transition::Transition(State *f, const std::string &l, State *t)
  : from(f), label(l), to(t) {}

NFA::Transition::Transition(State *f, std::string &&l, State *t)
  : from(f), label(std::move(l)), to(t) {}

NFA::NFA(const std::string &s)
  : states_(2)
  , transitions_{{&states_.front(), s, &states_.back()}} {}

NFA::NFA(std::string &&s)
  : states_(2)
  , transitions_{{&states_.front(), std::move(s), &states_.back()}} {}

void NFA::Merge(NFA &&other) {
  states_.splice(states_.end(), std::move(other.states_));
  transitions_.splice(transitions_.end(), std::move(other.transitions_));
}

void NFA::Concatenate(NFA &&other) {
  transitions_.emplace_back(&terminalState(), "", &other.initialState());
  Merge(std::move(other));
}

void NFA::Sum(NFA &&other) {
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

void NFA::IterateAtLeastOnce() {
  transitions_.emplace_back(&terminalState(), "", &initialState());
}

void NFA::Iterate() {
  transitions_.emplace_back(&terminalState(), "", &initialState());
  transitions_.emplace_back(&initialState(), "", &terminalState());
}

std::ostream &operator<<(std::ostream &out, const NFA &nfa) {
  for (const auto &trans : nfa.transitions_) {
    out << trans.from;
    out << " \"" << trans.label << "\" ";
    out << trans.to;
    out << std::endl;
  }
  return out;
}

}  // namespace magex
