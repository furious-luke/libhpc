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
#include <libhpc/numerics/spline_integrator.hh>
#include <libhpc/system/functional.hh>

using namespace hpc;
using namespace hpc::num;
using namespace std::placeholders;

TEST_CASE( "/libhpc/numerics/spline_integrator/flat" )
{
   spline<double> sp0;
   {
      std::vector<double> pnts( 3 ), vals( 3 );
      pnts[0] = -1.0; vals[0] = 1.0;
      pnts[1] =  0.0; vals[1] = 1.0;
      pnts[2] =  1.0; vals[2] = 1.0;
      sp0.set_knot_points( pnts );
      sp0.set_knot_values( vals );
      sp0.update();
   }

   spline_integrator<double> integ;
   double sum = integ( sp0, std::bind( identity<double>(), _2 ) );
   DELTA( sum, 2.0, 1e-6 );
}

TEST_CASE( "/libhpc/numerics/spline_integrator/flat_lambda" )
{
   spline<double> sp0;
   {
      std::vector<double> pnts( 3 ), vals( 3 );
      pnts[0] = -1.0; vals[0] = 1.0;
      pnts[1] =  0.0; vals[1] = 1.0;
      pnts[2] =  1.0; vals[2] = 1.0;
      sp0.set_knot_points( pnts );
      sp0.set_knot_values( vals );
      sp0.update();
   }

   spline_integrator<double> integ;
   double sum = integ(
      sp0,
      []( double x, double val )
      {
         return val*(1.0/x);
      }
      );
   DELTA( sum, 0.0, 1e-6 );
}

TEST_CASE( "/libhpc/numerics/spline_integrator/parabola" )
{
   spline<double> sp0;
   {
      std::vector<double> pnts( 9 ), vals( 9 );
      pnts[0] = -1.0;  vals[0] = 1.0;
      pnts[1] = -0.75; vals[1] = 0.5625;
      pnts[2] = -0.5;  vals[2] = 0.25;
      pnts[3] = -0.25; vals[3] = 0.0625;
      pnts[4] =  0.0;  vals[4] = 0.0;
      pnts[5] =  0.25; vals[5] = 0.0625;
      pnts[6] =  0.5;  vals[6] = 0.25;
      pnts[7] =  0.75; vals[7] = 0.5625;
      pnts[8] =  1.0;  vals[8] = 1.0;
      sp0.set_knot_points( pnts );
      sp0.set_knot_values( vals );
      sp0.update();
   }

   spline_integrator<double> integ;
   double sum = integ( sp0, std::bind( identity<double>(), _2 )  );
   DELTA( sum, 2.0/3.0, 1e-2 );
}

TEST_CASE( "/libhpc/numerics/spline_integrator/reverse" )
{
   spline<double> sp0;
   {
      std::vector<double> pnts( 9 ), vals( 9 );
      pnts[0] = -2.0; vals[0] = 1.0;
      pnts[1] = -1.5; vals[1] = 0.5625;
      pnts[2] = -1.0; vals[2] = 0.25;
      pnts[3] = -0.5; vals[3] = 0.0625;
      pnts[4] =  0.0; vals[4] = 0.0;
      pnts[5] =  0.5; vals[5] = 0.0625;
      pnts[6] =  1.0; vals[6] = 0.25;
      pnts[7] =  1.5; vals[7] = 0.5625;
      pnts[8] =  2.0; vals[8] = 1.0;
      sp0.set_knot_points( pnts );
      sp0.set_knot_values( vals );
      sp0.update();
   }

   double sum0;
   {
      spline_integrator<double> integ;
      sum0 = integ( sp0, 
                    []( double x, double val )
                    {
                       return val;
                    } );
   }
   double sum1;
   {
      spline_integrator<double> integ;
      sum1 = integ.reverse( sp0,
                            []( double x, double val )
                            {
                               return val;
                            } );
   }
   DELTA( sum0, sum1, 1e-8 );
}

TEST_CASE( "/libhpc/numerics/spline_integrator/transform" )
{
   spline<double> sp0;
   {
      std::vector<double> pnts( 9 ), vals( 9 );
      pnts[0] = 1.0; vals[0] = 1.0;
      pnts[1] = 1000.0; vals[1] = 1.0;
      pnts[2] = 2000.0; vals[2] = 1.0;
      pnts[3] = 3000.0; vals[3] = 1.0;
      pnts[4] = 4000.0; vals[4] = 1.0;
      pnts[5] = 5000.0; vals[5] = 1.0;
      pnts[6] = 6000.0; vals[6] = 1.0;
      pnts[7] = 7000.5; vals[7] = 1.0;
      pnts[8] = 8000.0; vals[8] = 1.0;
      sp0.set_knot_points( pnts );
      sp0.set_knot_values( vals );
      sp0.update();
   }

   spline_integrator<double> integ;
   double sum = integ(
      sp0, 
      []( double x, double val )
      {
         return 1.0;
      },
      []( double x )
      {
         return 1.0/x;
      }
      );
   DELTA( sum, -0.999875, 1e-4 );
}

TEST_CASE( "/libhpc/numerics/spline_spline_integrator/no_overlap" )
{
   spline<double> sp0;
   {
      std::vector<double> pnts( 3 ), vals( 3 );
      pnts[0] = -1.0; vals[0] = 1.0;
      pnts[1] =  0.0; vals[1] = 1.0;
      pnts[2] =  1.0; vals[2] = 1.0;
      sp0.set_knot_points( pnts );
      sp0.set_knot_values( vals );
      sp0.update();
   }

   spline<double> sp1;
   {
      std::vector<double> pnts( 3 ), vals( 3 );
      pnts[0] = 1.0; vals[0] = 1.0;
      pnts[1] = 2.0; vals[1] = 1.0;
      pnts[2] = 3.0; vals[2] = 1.0;
      sp1.set_knot_points( pnts );
      sp1.set_knot_values( vals );
      sp1.update();
   }

   spline_spline_integrator<double> integ;
   double sum = integ(
      sp0, sp1,
      std::bind( std::multiplies<double>(), _2, _3 )
      );
   TEST( sum == 0.0 );
}

TEST_CASE( "/libhpc/numerics/spline_spline_integrator/flat" )
{
   spline<double> sp0;
   {
      std::vector<double> pnts( 3 ), vals( 3 );
      pnts[0] = -1.0; vals[0] = 1.0;
      pnts[1] =  0.0; vals[1] = 1.0;
      pnts[2] =  1.0; vals[2] = 1.0;
      sp0.set_knot_points( pnts );
      sp0.set_knot_values( vals );
      sp0.update();
   }

   spline<double> sp1;
   {
      std::vector<double> pnts( 3 ), vals( 3 );
      pnts[0] = -1.0; vals[0] = 1.0;
      pnts[1] =  0.0; vals[1] = 1.0;
      pnts[2] =  1.0; vals[2] = 1.0;
      sp1.set_knot_points( pnts );
      sp1.set_knot_values( vals );
      sp1.update();
   }

   spline_spline_integrator<double> integ;
   double sum = integ(
      sp0, sp1,
      std::bind( std::multiplies<double>(), _2, _3 )
      );
   DELTA( sum, 2.0, 1e-6 );
}

TEST_CASE( "/libhpc/numerics/spline_spline_integrator/parabola" )
{
   spline<double> sp0;
   {
      std::vector<double> pnts( 5 ), vals( 5 );
      pnts[0] = -1.0; vals[0] = 1.0;
      pnts[1] = -0.5; vals[1] = 0.25;
      pnts[2] =  0.0; vals[2] = 0.0;
      pnts[3] =  0.5; vals[3] = 0.25;
      pnts[4] =  1.0; vals[4] = 1.0;
      sp0.set_knot_points( pnts );
      sp0.set_knot_values( vals );
      sp0.update();
   }

   spline<double> sp1;
   {
      std::vector<double> pnts( 5 ), vals( 5 );
      pnts[0] = -1.0; vals[0] = 1.0;
      pnts[1] = -0.5; vals[1] = 0.25;
      pnts[2] =  0.0; vals[2] = 0.0;
      pnts[3] =  0.5; vals[3] = 0.25;
      pnts[4] =  1.0; vals[4] = 1.0;
      sp1.set_knot_points( pnts );
      sp1.set_knot_values( vals );
      sp1.update();
   }

   spline_spline_integrator<double> integ;
   double sum = integ(
      sp0, sp1,
      std::bind( std::multiplies<double>(), _2, _3 )
      );
   DELTA( sum, 2.0/5.0, 1e-1 );
}
