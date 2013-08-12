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
#include "libhpc/containers/random.hh"

using namespace hpc;

class random_suite : public CxxTest::TestSuite {
public:

   void test_seed()
   {
      uniform_generator<double> real_gen;
      real_gen.set_seed( 1 );
      double vals[4];
      for( unsigned ii = 0; ii < 4; ++ii )
	 vals[ii] = real_gen();
      real_gen.set_seed( 1 );
      for( unsigned ii = 0; ii < 4; ++ii )
      {
	 double cur = real_gen();
	 TS_ASSERT_EQUALS( cur, vals[ii] );
      }
   }

   void setUp()
   {
   }
};
