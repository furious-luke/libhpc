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
#include "libhpc/debug/assert.hh"

using namespace hpc;

class assert_suite : public CxxTest::TestSuite {
public:

   void test_basic()
   {
      bool inside = false;
      try
      {
         ASSERT( 0 );
      }
#ifndef NDEBUG
      catch( debug::assertion& asrt )
#else
      catch( std::exception& asrt )
#endif
      {
         inside = true;
         TS_ASSERT( asrt.what() != NULL );
      }
#ifndef NDEBUG
      TS_ASSERT( inside );
#else
      TS_ASSERT( !inside );
#endif
   }

   void test_with_message()
   {
      bool inside = false;
      try
      {
         ASSERT( 0, "Something!" );
      }
#ifndef NDEBUG
      catch( debug::assertion& asrt )
#else
      catch( std::exception& asrt )
#endif
      {
         inside = true;
         TS_ASSERT( asrt.what() != NULL );
      }
#ifndef NDEBUG
      TS_ASSERT( inside );
#else
      TS_ASSERT( !inside );
#endif
   }
};
