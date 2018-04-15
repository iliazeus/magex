// Copyright 2018 Ilia Pozdnyakov
// This file is distributed under the MIT license.
// See the LICENSE.txt file for details.

#include "./parse.h"

namespace magex {

void PrecedenceParser::Reduce(Op op) {
  while (!op_stack_.empty() && op_stack_.top() > op) {
    switch (op_stack_.top()) {
      case Op::kEof:
        break;
      case Op::kSum: {
        if (value_stack_.size() < 2) {
          error_ = true;
          return;
        }
        NFA valTop = std::move(value_stack_.top());
        value_stack_.pop();
        value_stack_.top().Sum(std::move(valTop));
        break;
      }
      case Op::kConstant:
        break;
      case Op::kConcatenate: {
        if (value_stack_.size() < 2) {
          error_ = true;
          return;
        }
        NFA valTop = std::move(value_stack_.top());
        value_stack_.pop();
        value_stack_.top().Concatenate(std::move(valTop));
        break;
      }
      case Op::kIterate:
        if (value_stack_.empty()) {
          error_ = true;
          return;
        }
        value_stack_.top().Iterate();
        break;
      case Op::kIterateAtLeastOnce:
        if (value_stack_.empty()) {
          error_ = true;
          return;
        }
        value_stack_.top().IterateAtLeastOnce();
        break;
      case Op::kOptional:
        if (value_stack_.empty()) {
          error_ = true;
          return;
        }
        value_stack_.top().Sum(NFA(""));
        break;
      default:
        break;
    }
    op_stack_.pop();
  }
}

void PrecedenceParser::Consume(char c) {
  if (error_) return;
  switch (c) {
    case ')':
      Reduce(Op::kParenthesisLow);
      if (op_stack_.empty() || op_stack_.top() != Op::kParenthesisLow) {
        error_ = true;
        return;
      }
      op_stack_.pop();
      op_stack_.push(Op::kConstant);
      break;
    case '(':
      Reduce(Op::kParenthesisHigh);
      if (!value_stack_.empty()) {
        op_stack_.push(Op::kConcatenate);
      }
      op_stack_.push(Op::kParenthesisLow);
      break;
    case '|':
      Reduce(Op::kSum);
      op_stack_.push(Op::kSum);
      break;
    case '?':
      Reduce(Op::kOptional);
      if (op_stack_.empty() || op_stack_.top() == Op::kParenthesisLow) {
        error_ = true;
        break;
      }
      op_stack_.push(Op::kOptional);
      break;
    case '+':
      Reduce(Op::kIterateAtLeastOnce);
      if (op_stack_.empty() || op_stack_.top() == Op::kParenthesisLow) {
        error_ = true;
        break;
      }
      op_stack_.push(Op::kIterateAtLeastOnce);
      break;
    case '*':
      Reduce(Op::kIterate);
      if (op_stack_.empty() || op_stack_.top() == Op::kParenthesisLow) {
        error_ = true;
        break;
      }
      op_stack_.push(Op::kIterate);
      break;
    default:
      Reduce(Op::kConcatenate);
      if (!op_stack_.empty() && (op_stack_.top() == Op::kConcatenate ||
                                 op_stack_.top() == Op::kConstant)) {
        op_stack_.push(Op::kConcatenate);
      } else {
        op_stack_.push(Op::kConstant);
      }
      value_stack_.emplace(std::string(1, c));
  }
}

std::optional<NFA> PrecedenceParser::Result() {
  Reduce(Op::kEof);
  if (value_stack_.empty()) error_ = true;
  if (error_) return std::nullopt;
  NFA result = std::move(value_stack_.top());
  value_stack_.pop();
  return std::make_optional(std::move(result));
}

}  // namespace magex
