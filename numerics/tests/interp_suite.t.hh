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
#include "libhpc/containers/num.hh"
#include "libhpc/numerics/interp.hh"

using namespace hpc;

class interp_suite : public CxxTest::TestSuite {
public:

   void test_interpolate()
   {
      vector<double> abs( 4 ), vals( 4 );
      abs[0] = -1.0; vals[0] = 0.5;
      abs[1] = 0.0;  vals[1] = 0.0;
      abs[2] = 3.0;  vals[2] = 3.0;
      abs[3] = 5.0;  vals[3] = 2.0;

      numerics::interp<double> interp;
      interp.set_abscissa( abs );
      interp.set_values( vals );
      TS_ASSERT( abs.empty() );
      TS_ASSERT( vals.empty() );

      // Test specific values.
      TS_ASSERT_DELTA( interp[-1.0], 0.5, 1e-5 );
      TS_ASSERT_DELTA( interp[0.0], 0.0, 1e-5 );
      TS_ASSERT_DELTA( interp[3.0], 3.0, 1e-5 );
      TS_ASSERT_DELTA( interp[5.0], 2.0, 1e-5 );

      // Test interpolation.
      TS_ASSERT_DELTA( interp[-0.5], 0.25, 1e-5 );
      TS_ASSERT_DELTA( interp[1.5], 1.5, 1e-5 );
      TS_ASSERT_DELTA( interp[4.0], 2.5, 1e-5 );
   }
};
