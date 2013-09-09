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

#include <libhpc/debug/unit_test_main.hh>
#include "libhpc/numerics/spline.hh"
#include "libhpc/numerics/spline_integrator.hh"

using namespace hpc;
using namespace hpc::test;
using namespace hpc::numerics;
using namespace std::placeholders;

namespace {

   test_case<> ANON(
      "/numerics/spline_integrator/flat",
      "",
      []()
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
      );

   test_case<> ANON(
      "/numerics/spline_integrator/parabola",
      "",
      []()
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
      );

   test_case<> ANON(
      "/numerics/spline_spline_integrator/no_overlap",
      "",
      []()
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
      );

   test_case<> ANON(
      "/numerics/spline_spline_integrator/flat",
      "",
      []()
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
      );

   test_case<> ANON(
      "/numerics/spline_spline_integrator/parabola",
      "",
      []()
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
      );

}
