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
    : states_(2), transitions_{{&states_.front(), s, &states_.back()}} {}

NFA::NFA(std::string &&s)
    : states_(2),
      transitions_{{&states_.front(), std::move(s), &states_.back()}} {}

 NFA::NFA() : states_(2) {}

void NFA::Merge(NFA &&other) {
  states_.splice(states_.end(), std::move(other.states_));
  transitions_.splice(transitions_.end(), std::move(other.transitions_));
}

void NFA::Concatenate(NFA &&other) {
  transitions_.emplace_back(&terminal_state(), "", &other.initial_state());
  Merge(std::move(other));
}

void NFA::Sum(NFA &&other) {
  State *this_init = &initial_state();
  State *other_init = &other.initial_state();
  State *this_term = &terminal_state();
  State *other_term = &other.terminal_state();

  states_.emplace_front();
  transitions_.emplace_front(&initial_state(), "", this_init);
  transitions_.emplace_front(&initial_state(), "", other_init);

  Merge(std::move(other));

  states_.emplace_back();
  transitions_.emplace_back(this_term, "", &terminal_state());
  transitions_.emplace_back(other_term, "", &terminal_state());
}

void NFA::IterateAtLeastOnce() {
  transitions_.emplace_back(&terminal_state(), "", &initial_state());
}

void NFA::Iterate() {
  transitions_.emplace_back(&terminal_state(), "", &initial_state());
  transitions_.emplace_back(&initial_state(), "", &terminal_state());
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
