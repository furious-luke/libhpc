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
#include "libhpc/numerics/coords.hh"

using namespace hpc;
using namespace hpc::test;

namespace {

   test_case<> ANON(
      "/numerics/coords/cartesian_to_spherical",
      "",
      []()
      {
	 double r, theta, phi;
	 numerics::cartesian_to_spherical<double>( 1, 0, 0, r, theta, phi );
	 DELTA( r, 1.0, 1e-8 );
	 DELTA( phi, 0.0, 1e-8 );
	 DELTA( theta, 0.5*M_PI, 1e-8 );

	 numerics::cartesian_to_spherical<double>( 0, 1, 0, r, theta, phi );
	 DELTA( r, 1.0, 1e-8 );
	 DELTA( phi, 0.5*M_PI, 1e-8 );
	 DELTA( theta, 0.5*M_PI, 1e-8 );

	 numerics::cartesian_to_spherical<double>( 0, 0, 1, r, theta, phi );
	 DELTA( r, 1.0, 1e-8 );
	 DELTA( phi, 0.0, 1e-8 );
	 DELTA( theta, 0.0, 1e-8 );

	 numerics::cartesian_to_spherical<double>( 1, 1, 1, r, theta, phi );
	 DELTA( r, sqrt( 3.0 ), 1e-8 );
	 DELTA( phi, 0.25*M_PI, 1e-8 );
	 DELTA( theta, acos( 1.0/sqrt( 3.0 ) ), 1e-8 );
      }
      );

   test_case<> ANON(
      "/numerics/coords/cartesian_to_ecs",
      "",
      []()
      {
	 double ra, dec;
	 numerics::cartesian_to_ecs<double>( 1, 1, 1, ra, dec );
	 DELTA( ra, 0.25*M_PI, 1e-8 );
	 DELTA( dec, 0.5*M_PI - acos( 1.0/sqrt( 3.0 ) ), 1e-8 );
      }
      );

   test_case<> ANON(
      "/numerics/coords/gnomonic_projection",
      "",
      []()
      {
	 double x, y;
	 numerics::gnomonic_projection<double>( 0.0, 0.0, 0.0, 0.0, x, y );
	 DELTA( x, 0.0, 1e-8 );
	 DELTA( y, 0.0, 1e-8 );
	 numerics::gnomonic_projection<double>( 0.5, 0.5, 0.5, 0.5, x, y );
	 DELTA( x, 0.0, 1e-8 );
	 DELTA( y, 0.0, 1e-8 );
	 numerics::gnomonic_projection<double>( 0.5, -0.5, 0.5, -0.5, x, y );
	 DELTA( x, 0.0, 1e-8 );
	 DELTA( y, 0.0, 1e-8 );

	 // Nonzero results.
	 double old_x, old_y;
	 numerics::gnomonic_projection<double>( 0.0, 0.0, 0.5, 0.0, x, y );
	 TEST( x < 0.0 );
	 DELTA( y, 0.0, 1e-8 );
	 old_x = x;
	 numerics::gnomonic_projection<double>( 0.5, 0.0, 0.0, 0.0, x, y );
	 DELTA( x, -old_x, 1e-8 );
	 DELTA( y, 0.0, 1e-8 );
	 numerics::gnomonic_projection<double>( 0.0, 0.0, 0.0, 0.5, x, y );
	 DELTA( x, 0.0, 1e-8 );
	 TEST( y < 0.0 );
	 old_y = y;
	 numerics::gnomonic_projection<double>( 0.0, 0.5, 0.0, 0.0, x, y );
	 DELTA( x, 0.0, 1e-8 );
	 DELTA( y, -old_y, 1e-8 );

	 // More distant points should increase faster in each dimension.
	 double diff[2];
	 numerics::gnomonic_projection<double>( 0.0, 0.0, 0.1, 0.0, x, y );
	 diff[0] = x;
	 old_x = x;
	 numerics::gnomonic_projection<double>( 0.0, 0.0, 0.2, 0.0, x, y );
	 diff[1] = x - old_x;
	 TEST( diff[1] < diff[0] );
	 old_x = x;
	 numerics::gnomonic_projection<double>( 0.0, 0.0, 0.3, 0.0, x, y );
	 diff[0] = x - old_x;
	 TEST( diff[0] < diff[1] );
	 numerics::gnomonic_projection<double>( 0.0, 0.1, 0.0, 0.0, x, y );
	 diff[0] = y;
	 old_y = y;
	 numerics::gnomonic_projection<double>( 0.0, 0.2, 0.0, 0.0, x, y );
	 diff[1] = y - old_y;
	 TEST( diff[1] > diff[0] );
	 old_y = y;
	 numerics::gnomonic_projection<double>( 0.0, 0.3, 0.0, 0.0, x, y );
	 diff[0] = y - old_y;
	 TEST( diff[0] > diff[1] );
      }
      );

   test_case<> ANON(
      "/numerics/coords/redshift_to_comoving_distance",
      "",
      []()
      {
	 double dist = numerics::redshift_to_comoving_distance<double>( 3, 1000, 73, 0.75, 0.25 );
	 DELTA( dist, 4690.3/0.73, 10 );
      }
      );

   test_case<> ANON(
      "/numerics/coords/redshift_to_light_travel_distance",
      "",
      []()
      {
	 double dist = numerics::redshift_to_light_travel_distance( 0.02, 1000 );
	 double age = dist/constant::c_mpc_gyr;
	 DELTA( age, 0.272, 1e-2 );
      }
      );

   test_case<> ANON(
      "/numerics/coords/redshift_to_light_luminosity_distance",
      "",
      []()
      {
	 double dist = numerics::redshift_to_luminosity_distance<double>( 3, 1000, 73, 0.75, 0.25 );
	 DELTA( dist, 25694.0, 10 );
      }
      );

}
