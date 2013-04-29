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
#include "libhpc/algorithm/ridders.hh"

using namespace hpc;

double
f_sq( double x )
{
   return (x + 1.0)*(x + 1.0) - 1.0;
}

class ridders_suite : public CxxTest::TestSuite {
public:

   void test_initial_bracketing()
   {
#ifndef NDEBUG
      TS_ASSERT_THROWS_ANYTHING( algorithm::ridders( f_sq, -10.0, 2.0 ) );
#endif
   }

   void test_quadratic()
   {
      double root = algorithm::ridders( f_sq, -10.0, -0.5 );
      TS_ASSERT (root >= -2.0 - algorithm::default_xtol && root <= -2.0 + algorithm::default_xtol );
   }

   void test_tolerance()
   {
      unsigned num_base_its, num_its;
      double cur_xtol = algorithm::default_xtol/100.0;
      double root = algorithm::ridders( f_sq, -10.0, -0.5, cur_xtol, num_base_its );
      TS_ASSERT( root >= -2.0 - cur_xtol && root <= -2.0 + cur_xtol );

      cur_xtol = 10000.0*algorithm::default_xtol;
      root = algorithm::ridders( f_sq, -10.0, -0.5, cur_xtol, num_its );
      TS_ASSERT( root >= -2.0 - cur_xtol && root <= -2.0 + cur_xtol );
      TS_ASSERT( num_its < num_base_its );
   }
};
