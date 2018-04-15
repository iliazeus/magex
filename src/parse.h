// Copyright 2018 Ilia Pozdnyakov
// This file is distributed under the MIT license.
// See the LICENSE.txt file for details.

#ifndef PARSE_H_
#define PARSE_H_

#include <optional>
#include <stack>
#include <string>

#include "./nfa.h"

namespace magex {

class PrecedenceParser {
 public:
  void Consume(char c);
  std::optional<NFA> Result();

 protected:
  enum class Op {
    kEof,
    kParenthesisLow,
    kSum,
    kConstant,
    kParenthesisHigh,
    kConcatenate,
    kIterateAtLeastOnce,
    kIterate,
    kOptional,
  };
  std::stack<NFA> value_stack_;
  std::stack<Op> op_stack_;
  bool error_ = false;

 private:
  void Reduce(Op op);
};

template <class CharIt>
std::optional<NFA> ParseRegexPrecedence(CharIt begin, CharIt end) {
  PrecedenceParser parser;
  for (auto it = begin; it != end; ++it) parser.Consume(*it);
  return parser.Result();
}

template <class CharIt>
std::optional<NFA> ParseRegex(CharIt begin, CharIt end) {
  return ParseRegexPrecedence(begin, end);
}
inline std::optional<NFA> ParseRegex(const std::string &s) {
  return ParseRegex(s.begin(), s.end());
}

}  // namespace magex

#endif  // PARSE_H_
