// Copyright 2018 Ilia Pozdnyakov
// This file is distributed under the MIT license.
// See the LICENSE.txt file for details.

#include "./parse.h"

namespace magex {

void PrecedenceParser::Reduce(Op op) {
  while (!opStack_.empty() && opStack_.top() > op) {
    switch (opStack_.top()) {
      case Op::kEof:
        break;

      case Op::kSum:
        {
          if (valueStack_.size() < 2) { error_ = true; return; }
          NFA valTop = std::move(valueStack_.top());
          valueStack_.pop();
          valueStack_.top().Sum(std::move(valTop));
        }
        break;

      case Op::kConstant:
        break;

      case Op::kConcatenate:
        {
          if (valueStack_.size() < 2) { error_ = true; return; }
          NFA valTop = std::move(valueStack_.top());
          valueStack_.pop();
          valueStack_.top().Concatenate(std::move(valTop));
        }
        break;

      case Op::kIterate:
        if (valueStack_.empty()) { error_ = true; return; }
        valueStack_.top().Iterate();
        break;

      case Op::kIterateAtLeastOnce:
        if (valueStack_.empty()) { error_ = true; return; }
        valueStack_.top().IterateAtLeastOnce();
        break;

      case Op::kOptional:
        if (valueStack_.empty()) { error_ = true; return; }
        valueStack_.top().Sum(NFA(""));
        break;

       default:
        break;
    }
    opStack_.pop();
  }
}

void PrecedenceParser::Consume(char c) {
  if (error_) return;
  switch (c) {
    case ')':
      Reduce(Op::kParenthesisLow);
      if (opStack_.empty() || opStack_.top() != Op::kParenthesisLow) {
        error_ = true; return;
      }
      opStack_.pop();
      opStack_.push(Op::kConstant);
      break;
     case '(':
      Reduce(Op::kParenthesisHigh);
      if (!valueStack_.empty()) {
        opStack_.push(Op::kConcatenate);
      }
      opStack_.push(Op::kParenthesisLow);
      break;
    case '|':
      Reduce(Op::kSum);
      opStack_.push(Op::kSum);
      break;
    case '?':
      Reduce(Op::kOptional);
      if (opStack_.empty() || opStack_.top() == Op::kParenthesisLow) {
        error_ = true; break;
      }
      opStack_.push(Op::kOptional);
      break;
    case '+':
      Reduce(Op::kIterateAtLeastOnce);
      if (opStack_.empty() || opStack_.top() == Op::kParenthesisLow) {
        error_ = true; break;
      }
      opStack_.push(Op::kIterateAtLeastOnce);
      break;
    case '*':
      Reduce(Op::kIterate);
      if (opStack_.empty() || opStack_.top() == Op::kParenthesisLow) {
        error_ = true; break;
      }
      opStack_.push(Op::kIterate);
      break;
     default:
      Reduce(Op::kConcatenate);
      if (!opStack_.empty() && (opStack_.top() == Op::kConcatenate
                                || opStack_.top() == Op::kConstant)) {
        opStack_.push(Op::kConcatenate);
      } else {
        opStack_.push(Op::kConstant);
      }
      valueStack_.emplace(std::string(1, c));
  }
}

std::optional<NFA> PrecedenceParser::Result() {
  Reduce(Op::kEof);
  if (valueStack_.empty()) error_ = true;
  if (error_) return std::nullopt;
  NFA result = std::move(valueStack_.top());
  valueStack_.pop();
  return std::make_optional(std::move(result));
}

}  // namespace magex
