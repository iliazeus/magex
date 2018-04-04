// Copyright 2018 Ilia Pozdnyakov
// This file is distributed under the MIT license.
// See the LICENSE.txt file for details.

#include "../src/parse.h"

#include <gtest/gtest.h>

#include <iostream>

using magex::NFA;
using magex::ParseRegex;

class ParseTest : public ::testing::Test {};

TEST_F(ParseTest, SingleWord) {
  NFA nfa = ParseRegex("foobar").value();
  EXPECT_TRUE(nfa.Accepts("foobar"));
  EXPECT_FALSE(nfa.Accepts("bazbaz"));
}

TEST_F(ParseTest, Iterate) {
  NFA nfa = ParseRegex("ba*r").value();
  EXPECT_TRUE(nfa.Accepts("br"));
  EXPECT_TRUE(nfa.Accepts("bar"));
  EXPECT_TRUE(nfa.Accepts("baaar"));

  EXPECT_FALSE(ParseRegex("*foo").has_value());
}

TEST_F(ParseTest, IterateAtLeastOnce) {
  NFA nfa = ParseRegex("ba+r").value();
  EXPECT_FALSE(nfa.Accepts("br"));
  EXPECT_TRUE(nfa.Accepts("bar"));
  EXPECT_TRUE(nfa.Accepts("baaar"));

  EXPECT_FALSE(ParseRegex("+foo").has_value());
}

TEST_F(ParseTest, Optional) {
  NFA nfa = ParseRegex("ba?r").value();
  EXPECT_TRUE(nfa.Accepts("br"));
  EXPECT_TRUE(nfa.Accepts("bar"));
  EXPECT_FALSE(nfa.Accepts("baaar"));

  EXPECT_FALSE(ParseRegex("?foo").has_value());
}

TEST_F(ParseTest, Sum) {
  NFA nfa = ParseRegex("foo|bar|baz").value();
  EXPECT_TRUE(nfa.Accepts("foo"));
  EXPECT_TRUE(nfa.Accepts("bar"));
  EXPECT_TRUE(nfa.Accepts("baz"));

  EXPECT_FALSE(ParseRegex("|foo").has_value());
  EXPECT_FALSE(ParseRegex("foo|").has_value());
  EXPECT_FALSE(ParseRegex("foo||bar").has_value());
}

TEST_F(ParseTest, ParenthesisSimple) {
  NFA nfa = ParseRegex("(foo)").value();
  EXPECT_TRUE(nfa.Accepts("foo"));
  EXPECT_FALSE(nfa.Accepts("(foo)"));
}

TEST_F(ParseTest, ParenthesisConcatenation) {
  NFA nfa = ParseRegex("f(b)z").value();
  EXPECT_TRUE(nfa.Accepts("fbz"));
  EXPECT_FALSE(nfa.Accepts("f(b)z"));
}

TEST_F(ParseTest, ParenthesisIteration) {
  NFA nfa = ParseRegex("(ab?)+").value();
  EXPECT_TRUE(nfa.Accepts("ab"));
  EXPECT_TRUE(nfa.Accepts("abab"));
  EXPECT_TRUE(nfa.Accepts("aab"));
  EXPECT_FALSE(nfa.Accepts("abb"));
}

TEST_F(ParseTest, Complex1) {
  NFA nfa = ParseRegex("(0|1)+(-(0|1)+)?").value();
  EXPECT_TRUE(nfa.Accepts("100"));
  EXPECT_TRUE(nfa.Accepts("10-10"));
  EXPECT_TRUE(nfa.Accepts("0-001"));
  EXPECT_FALSE(nfa.Accepts("-010"));
  EXPECT_FALSE(nfa.Accepts("111-"));
}

TEST_F(ParseTest, ComplexInvalid) {
  EXPECT_FALSE(ParseRegex("").has_value());
  EXPECT_FALSE(ParseRegex("(a))").has_value());
  EXPECT_FALSE(ParseRegex("a(+b)").has_value());
  EXPECT_FALSE(ParseRegex("()").has_value());
  EXPECT_FALSE(ParseRegex("(()?()+)*").has_value());
}
