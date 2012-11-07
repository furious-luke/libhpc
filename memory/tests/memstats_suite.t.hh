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

#include <numeric>
#include <cxxtest/TestSuite.h>
#include "libhpc/memory/new.hh"
#include "libhpc/memory/group_context.hh"
#include "libhpc/memory/state.hh"

using namespace hpc;

namespace hpc {
   namespace memory {
      extern group_context<state_t> ctx;
   }
}

class memstats_suite : public CxxTest::TestSuite {
public:

   void test_update()
   {
#ifndef NMEMSTATS
      auto cur_size = memory::ctx.find_group( "/" ).data().size;
      auto cur_peak = memory::ctx.find_group( "/" ).data().peak;
      int* buf = new int[100];
      TS_ASSERT_EQUALS( memory::ctx.find_group( "/" ).data().size, cur_size + 100*sizeof(int) );
      TS_ASSERT_EQUALS( memory::ctx.find_group( "/" ).data().peak, cur_peak + 100*sizeof(int) );
#endif
   }
};
