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
#include "libhpc/numerics/spline.hh"

using namespace hpc;

class spline_suite : public CxxTest::TestSuite {
public:

   void test_three_point_solve()
   {
      fibre<double> knots( 2, 3 );
      knots(0,0) = -1.0; knots(0,1) = 0.5;
      knots(1,0) = 0.0; knots(1,1) = 0.0;
      knots(2,0) = 3.0; knots(2,1) = 3.0;

      numerics::spline<double> spline;
      spline.set_knots( knots );
      TS_ASSERT( num::approx<double>( spline._ai[0], -0.1875, 0.00001 ) );
      TS_ASSERT( num::approx<double>( spline._ai[1], -3.3750, 0.00001 ) );
      TS_ASSERT( num::approx<double>( spline._bi[0], -0.3750, 0.00001 ) );
      TS_ASSERT( num::approx<double>( spline._bi[1], -1.6875, 0.00001 ) );
   }

   void test_interpolate()
   {
      fibre<double> knots( 2, 4 );
      knots(0,0) = -1.0; knots(0,1) = 0.5;
      knots(1,0) = 0.0; knots(1,1) = 0.0;
      knots(2,0) = 3.0; knots(2,1) = 3.0;
      knots(3,0) = 5.0; knots(3,1) = 2.0;

      numerics::spline<double> spline;
      spline.set_knots( knots );
      for( unsigned ii = 0; ii <= 100; ++ii )
      {
         double x = -1.0 + 6.0*((double)ii)/100.0;
         spline( x );
      }
   }
};
