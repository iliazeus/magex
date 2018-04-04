// Copyright 2018 Ilia Pozdnyakov
// This file is distributed under the MIT license.
// See the LICENSE.txt file for details.

#include "../src/nfa.h"

#include <gtest/gtest.h>

using magex::NFA;

class NFATest : public ::testing::Test {};

TEST_F(NFATest, AcceptsWorks) {
  NFA nfa1("foobar");
  EXPECT_TRUE(nfa1.Accepts("foobar"));
  EXPECT_FALSE(nfa1.Accepts("bazbaz"));
}

TEST_F(NFATest, ConcatenateWorks) {
  NFA nfa1("foo");
  nfa1.Concatenate(NFA("bar"));
  EXPECT_TRUE(nfa1.Accepts("foobar"));
  EXPECT_FALSE(nfa1.Accepts("foo"));
  EXPECT_FALSE(nfa1.Accepts("bar"));
}

TEST_F(NFATest, SumWorks) {
  NFA nfa1("foo");
  nfa1.Sum(NFA("bar"));
  EXPECT_FALSE(nfa1.Accepts("foobar"));
  EXPECT_TRUE(nfa1.Accepts("foo"));
  EXPECT_TRUE(nfa1.Accepts("bar"));
}

TEST_F(NFATest, IterateWorks) {
  NFA nfa1("foo");
  nfa1.Iterate();
  EXPECT_TRUE(nfa1.Accepts(""));
  EXPECT_TRUE(nfa1.Accepts("foo"));
  EXPECT_TRUE(nfa1.Accepts("foofoofoo"));
}

TEST_F(NFATest, IterateAtLeastOnceWorks) {
  NFA nfa1("foo");
  nfa1.IterateAtLeastOnce();
  EXPECT_FALSE(nfa1.Accepts(""));
  EXPECT_TRUE(nfa1.Accepts("foo"));
  EXPECT_TRUE(nfa1.Accepts("foofoofoo"));
}
