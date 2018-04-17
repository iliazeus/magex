// Copyright 2018 Ilia Pozdnyakov
// This file is distributed under the MIT license.
// See the LICENSE.txt file for details.

#include "./dfa.h"

namespace magex {

std::ostream &operator<<(std::ostream &out, const DFA &dfa) {
  for (const auto &row : dfa.table_) {
    for (unsigned char c = 'a'; c <= 'z'; c++) {
      if (row[c] >= dfa.table_.size())
        out << '.';
      else
        out << row[c];
      out << ' ';
    }
    out << std::endl;
  }
  return out;
}

}  // namespace magex