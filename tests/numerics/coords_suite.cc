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
#include <libhpc/numerics/coords.hh>

TEST_CASE( "/libhpc/numerics/coords/cartesian_to_spherical" )
{
   double r, theta, phi;
   hpc::num::cartesian_to_spherical<double>( 1, 0, 0, r, theta, phi );
   DELTA( r, 1.0, 1e-8 );
   DELTA( phi, 0.0, 1e-8 );
   DELTA( theta, 0.5*M_PI, 1e-8 );

   hpc::num::cartesian_to_spherical<double>( 0, 1, 0, r, theta, phi );
   DELTA( r, 1.0, 1e-8 );
   DELTA( phi, 0.5*M_PI, 1e-8 );
   DELTA( theta, 0.5*M_PI, 1e-8 );

   hpc::num::cartesian_to_spherical<double>( 0, 0, 1, r, theta, phi );
   DELTA( r, 1.0, 1e-8 );
   DELTA( phi, 0.0, 1e-8 );
   DELTA( theta, 0.0, 1e-8 );

   hpc::num::cartesian_to_spherical<double>( 1, 1, 1, r, theta, phi );
   DELTA( r, sqrt( 3.0 ), 1e-8 );
   DELTA( phi, 0.25*M_PI, 1e-8 );
   DELTA( theta, acos( 1.0/sqrt( 3.0 ) ), 1e-8 );
}

TEST_CASE( "/libhpc/numerics/coords/cartesian_to_ecs" )
{
   double ra, dec;

   // First quadrant.
   hpc::num::cartesian_to_ecs<double>( 1, 1, 1, ra, dec );
   DELTA( ra, 0.25*M_PI, 1e-8 );
   DELTA( dec, 0.5*M_PI - acos( 1.0/sqrt( 3.0 ) ), 1e-8 );

   // Second quadrant.
   hpc::num::cartesian_to_ecs<double>( -1, 1, 1, ra, dec );
   DELTA( ra, 0.75*M_PI, 1e-8 );
   DELTA( dec, 0.5*M_PI - acos( 1.0/sqrt( 3.0 ) ), 1e-8 );

   // Third quadrant.
   hpc::num::cartesian_to_ecs<double>( -1, -1, 1, ra, dec );
   DELTA( ra, 1.25*M_PI, 1e-8 );
   DELTA( dec, 0.5*M_PI - acos( 1.0/sqrt( 3.0 ) ), 1e-8 );

   // Fourth quadrant.
   hpc::num::cartesian_to_ecs<double>( 1, -1, 1, ra, dec );
   DELTA( ra, 1.75*M_PI, 1e-8 );
   DELTA( dec, 0.5*M_PI - acos( 1.0/sqrt( 3.0 ) ), 1e-8 );
}

TEST_CASE( "/libhpc/numerics/coords/ecs_to_cartesian" )
{
   double x, y, z, dec = 0.5*M_PI - acos( 1.0/sqrt( 3.0 ) );

   // First quadrant.
   hpc::num::ecs_to_cartesian<double>( 0.25*M_PI, dec, x, y, z, sqrt( 3.0 )  );
   DELTA( x, 1.0, 1e-8 );
   DELTA( y, 1.0, 1e-8 );
   DELTA( z, 1.0, 1e-8 );

   // Second quadrant.
   hpc::num::ecs_to_cartesian<double>( 0.75*M_PI, dec, x, y, z, sqrt( 3.0 )  );
   DELTA( x, -1.0, 1e-8 );
   DELTA( y, 1.0, 1e-8 );
   DELTA( z, 1.0, 1e-8 );

   // Third quadrant.
   hpc::num::ecs_to_cartesian<double>( 1.25*M_PI, dec, x, y, z, sqrt( 3.0 )  );
   DELTA( x, -1.0, 1e-8 );
   DELTA( y, -1.0, 1e-8 );
   DELTA( z, 1.0, 1e-8 );

   // Fourth quadrant.
   hpc::num::ecs_to_cartesian<double>( 1.75*M_PI, dec, x, y, z, sqrt( 3.0 )  );
   DELTA( x, 1.0, 1e-8 );
   DELTA( y, -1.0, 1e-8 );
   DELTA( z, 1.0, 1e-8 );
}

TEST_CASE( "/libhpc/numerics/coords/gnomonic_projection" )
{
   double x, y;
   hpc::num::gnomonic_projection<double>( 0.0, 0.0, 0.0, 0.0, x, y );
   DELTA( x, 0.0, 1e-8 );
   DELTA( y, 0.0, 1e-8 );
   hpc::num::gnomonic_projection<double>( 0.5, 0.5, 0.5, 0.5, x, y );
   DELTA( x, 0.0, 1e-8 );
   DELTA( y, 0.0, 1e-8 );
   hpc::num::gnomonic_projection<double>( 0.5, -0.5, 0.5, -0.5, x, y );
   DELTA( x, 0.0, 1e-8 );
   DELTA( y, 0.0, 1e-8 );

   // Nonzero results.
   double old_x, old_y;
   hpc::num::gnomonic_projection<double>( 0.0, 0.0, 0.5, 0.0, x, y );
   TEST( x < 0.0 );
   DELTA( y, 0.0, 1e-8 );
   old_x = x;
   hpc::num::gnomonic_projection<double>( 0.5, 0.0, 0.0, 0.0, x, y );
   DELTA( x, -old_x, 1e-8 );
   DELTA( y, 0.0, 1e-8 );
   hpc::num::gnomonic_projection<double>( 0.0, 0.0, 0.0, 0.5, x, y );
   DELTA( x, 0.0, 1e-8 );
   TEST( y < 0.0 );
   old_y = y;
   hpc::num::gnomonic_projection<double>( 0.0, 0.5, 0.0, 0.0, x, y );
   DELTA( x, 0.0, 1e-8 );
   DELTA( y, -old_y, 1e-8 );

   // More distant points should increase faster in each dimension.
   double diff[2];
   hpc::num::gnomonic_projection<double>( 0.0, 0.0, 0.1, 0.0, x, y );
   diff[0] = x;
   old_x = x;
   hpc::num::gnomonic_projection<double>( 0.0, 0.0, 0.2, 0.0, x, y );
   diff[1] = x - old_x;
   TEST( diff[1] < diff[0] );
   old_x = x;
   hpc::num::gnomonic_projection<double>( 0.0, 0.0, 0.3, 0.0, x, y );
   diff[0] = x - old_x;
   TEST( diff[0] < diff[1] );
   hpc::num::gnomonic_projection<double>( 0.0, 0.1, 0.0, 0.0, x, y );
   diff[0] = y;
   old_y = y;
   hpc::num::gnomonic_projection<double>( 0.0, 0.2, 0.0, 0.0, x, y );
   diff[1] = y - old_y;
   TEST( diff[1] > diff[0] );
   old_y = y;
   hpc::num::gnomonic_projection<double>( 0.0, 0.3, 0.0, 0.0, x, y );
   diff[0] = y - old_y;
   TEST( diff[0] > diff[1] );
}

TEST_CASE( "/libhpc/numerics/coords/redshift_to_comoving_distance" )
{
   double dist = hpc::num::redshift_to_comoving_distance<double>( 3, 1000, 73, 0.75, 0.25 );
   DELTA( dist, 4690.3/0.73, 10 );
}

TEST_CASE( "/libhpc/numerics/coords/redshift_to_light_travel_distance" )
{
   double dist = hpc::num::redshift_to_light_travel_distance( 0.02, 1000 );
   double age = dist/hpc::constant::c_mpc_gyr;
   DELTA( age, 0.272, 1e-2 );
}

TEST_CASE( "/libhpc/numerics/coords/redshift_to_light_luminosity_distance" )
{
   double dist = hpc::num::redshift_to_luminosity_distance<double>( 3, 1000, 73, 0.75, 0.25 );
   DELTA( dist, 25694.0, 10 );
}
