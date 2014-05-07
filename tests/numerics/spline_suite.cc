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

#include <libhpc/unit_test/main.hh>
#include <libhpc/numerics/spline.hh>

TEST_CASE( "/libhpc/numerics/spline/move_knots" )
{
   std::vector<double> pnts( 3 ), vals( 3 );
   pnts[0] = -1.0; vals[0] = 0.5;
   pnts[1] =  0.0; vals[1] = 0.0;
   pnts[2] =  3.0; vals[2] = 3.0;

   hpc::num::spline<double,std::vector<double>,std::vector<double>> spl;
   spl.set_knot_points( std::move( pnts ) );
   spl.set_knot_values( std::move( vals ) );

   TEST( pnts.empty() == true );
   TEST( vals.empty() == true );
   TEST( spl.knot_points()[0] == -1.0 );
   TEST( spl.knot_values()[0] == 0.5 );
}

TEST_CASE( "/libhpc/numerics/spline/three_point_solve" )
{
   std::vector<double> pnts( 3 ), vals( 3 );
   pnts[0] = -1.0; vals[0] = 0.5;
   pnts[1] =  0.0; vals[1] = 0.0;
   pnts[2] =  3.0; vals[2] = 3.0;

   hpc::num::spline<double> spl;
   spl.set_knot_points( pnts );
   spl.set_knot_values( vals );
   spl.update();

   DELTA( spl.ai()[0], -0.1875, 0.00001 );
   DELTA( spl.ai()[1], -3.3750, 0.00001 );
   DELTA( spl.bi()[0], -0.3750, 0.00001 );
   DELTA( spl.bi()[1], -1.6875, 0.00001 );
}

TEST_CASE( "/libhpc/numerics/spline/interpolate" )
{
   std::vector<double> pnts( 3 ), vals( 3 );
   pnts[0] = -1.0; vals[0] = 0.5;
   pnts[1] =  0.0; vals[1] = 0.0;
   pnts[2] =  3.0; vals[2] = 3.0;

   hpc::num::spline<double> spl;
   spl.set_knot_points( pnts );
   spl.set_knot_values( vals );
   spl.update();

   for( unsigned ii = 0; ii <= 100; ++ii )
   {
      double x = -1.0 + 6.0*((double)ii)/100.0;
      spl( x );
   }
}

TEST_CASE( "/libhpc/numerics/spline/shared_points" )
{
   std::vector<double> pnts( 3 ), vals( 3 );
   pnts[0] = -1.0; vals[0] = 0.5;
   pnts[1] =  0.0; vals[1] = 0.0;
   pnts[2] =  3.0; vals[2] = 3.0;

   hpc::num::spline<double,hpc::view<std::vector<double>>> spl;
   spl.set_knot_points( pnts );
   spl.set_knot_values( vals );

   TEST( pnts.size() == 3 );
   TEST( spl.knot_points().size() == 3 );
   TEST( spl.knot_points().data() == pnts.data() );
}
