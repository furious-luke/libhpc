// Copyright 2012 Luke Hodkinson

// This file is part of libhpc.
// 
// libhpc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// libhpc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with libhpc.  If not, see <http://www.gnu.org/licenses/>.

#include <cxxtest/TestSuite.h>
#include "libhpc/containers/multimap.hh"

using namespace hpc;

class multimap_suite : public CxxTest::TestSuite {
public:

  void test_default_ctor() {
    multimap<int, int> multimap;
    this->check_empty(multimap);
  }

  void check_empty(multimap<int, int>& multimap) {
    TS_ASSERT(multimap.empty());
  }
};
