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

#include <string.h>
#include <unistd.h>
#include <cxxtest/TestSuite.h>
#include "libhpc/system/timer.hh"

using namespace hpc;

class timer_suite : public CxxTest::TestSuite {
public:

   void test_wait()
   {
      posix::time_type start, finish;
      start = posix::timer();
      usleep( 1000000 ); // sleep for 1 second
      finish = posix::timer();
      TS_ASSERT_EQUALS( (finish - start).tv_sec, 1 );
      TS_ASSERT( (finish - start).tv_nsec < 1000000 );
   }
};
