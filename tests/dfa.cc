// Copyright 2018 Ilia Pozdnyakov
// This file is distributed under the MIT license.
// See the LICENSE.txt file for details.

#include "../src/dfa.h"

#include <gtest/gtest.h>

using magex::DFA;

class DFATest : public ::testing::Test {
 protected:
  DFATest() {
    auto &table = dfa_.table_for_testing();
    table.resize(4);
    for (auto &row : table) row.fill(dfa_.no_transition());
    table[0]['f'] = 1;
    table[1]['o'] = 2;
    table[2]['o'] = 3;
    // terminal states
    table[2][0] = 0;
    table[3][0] = 0;
  }

  DFA dfa_;
};

TEST_F(DFATest, AcceptsWorks) {
  EXPECT_TRUE(this->dfa_.Accepts("foo"));
  EXPECT_TRUE(this->dfa_.Accepts("fo"));
  EXPECT_FALSE(this->dfa_.Accepts("bar"));
  EXPECT_FALSE(this->dfa_.Accepts("fooo"));
}