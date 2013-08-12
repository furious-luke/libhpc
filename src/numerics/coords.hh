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

#ifndef libhpc_numerics_coords_hh
#define libhpc_numerics_coords_hh

#include "libhpc/debug/debug.hh"
#include "libhpc/containers/num.hh"
#include "libhpc/containers/optional.hh"
#include "constants.hh"
#include "simpson.hh"

namespace hpc {
   namespace numerics {

      ///
      /// Spherical coordinates in common physics interpretation, theta being
      /// the angle between the z axis and r. Phi being the angle between the
      /// x axis and the y axis.
      ///
      /// theta = inclination
      /// phi = azimuth
      ///
      template< class T >
      void
      cartesian_to_spherical( T x, T y, T z,
                              T& r, T& theta, T& phi )
      {
         r = sqrt( x*x + y*y + z*z );
         phi = atan2( y, x );
         if( !num::approx( r, 0.0 ) )
            theta = acos( z/r );
         else
            theta = 0.0;
      }

      ///
      /// Spherical coordinates in common physics interpretation, theta being
      /// the angle between the z axis and r. Phi being the angle between the
      /// x axis and the y axis.
      ///
      /// theta = inclination
      /// phi = azimuth
      ///
      template< class T >
      void
      spherical_to_cartesian( T r, T theta, T phi,
                              T& x, T& y, T& z )
      {
         T sint = sin( theta );
         x = r*sint*cos( phi );
         y = r*sint*sin( phi );
         z = r*cos( theta );
      }

      ///
      /// Convert cartesian coordinates to ECS. RA is the angle between
      /// the x and y axis, and DEC is the angle between the equatorial
      /// plane in (1, 1, 0) and the radial vector (x, y, z).
      ///
      template< class T >
      void
      cartesian_to_ecs( T x, T y, T z,
                        T& ra, T& dec,
                        optional<T&> r = optional<T&>() )
      {
         T _r;
         cartesian_to_spherical( x, y, z, _r, dec, ra );
         dec = 0.5*M_PI - dec;
         if( r )
            *r = _r;
      }

      ///
      /// Spherical coordinates in common physics interpretation, theta being
      /// the angle between the z axis and r. Phi being the angle between the
      /// x axis and the y axis.
      ///
      /// theta = inclination
      /// phi = azimuth
      ///
      template< class T >
      void
      ecs_to_cartesian( T ra, T dec,
                        T& x, T& y, T& z,
                        optional<T> rad = optional<T>() )
                        
      {
         dec = 0.5*M_PI - dec;
         T _rad = rad ? *rad : 1;
         spherical_to_cartesian( _rad, dec, ra, x, y, z );
      }

      template< class T >
      void
      gnomonic_projection( T object_ra, T object_dec,
                           T origin_ra, T origin_dec,
                           T& x, T& y )
      {
         T sind = sin( object_dec ), cosd = cos( object_dec );
         T sind0 = sin( origin_dec ), cosd0 = cos( origin_dec );
         T cosa = cos( object_ra - origin_ra );
         T cosc = sind0*sind + cosd0*cosd*cosa;
         x = cosd*sin( object_ra - origin_ra )/cosc;
         y = (cosd0*sind - sind0*cosd*cosa)/cosc;
      }

      template< class T >
      void
      inverse_gnomonic_projection( T x, T y,
                                   T origin_ra, T origin_dec,
                                   T& ra, T& dec )
      {
         T sind0 = sin( origin_dec ), cosd0 = cos( origin_dec );
         T rho = sqrt( x*x + y*y ), c = atan( rho );
         T sinc = sin( c ), cosc = cos( c );
         ASSERT( rho > 0.0 );
         ra = origin_ra + atan2( x*sinc, rho*cosd0*cosc - y*sind0*sinc );
         dec = asin( cosc*sind0 + y*sinc*cosd0/rho );
      }

      template< class T >
      struct redshift_to_comoving_distance_func
      {
         typedef T value_type;

         redshift_to_comoving_distance_func( T omega_v = 0.73,
					     T omega_m = 0.27 )
            : omega_v( omega_v ),
	      omega_m( omega_m ),
              omega_k( 1.0 - omega_m - omega_v )
         {
         }

         T
         operator()( T z ) const
         {
	    double z_sq = (1.0 + z)*(1.0 + z);
	    double e_z = sqrt( z_sq*(1.0 + z)*omega_m + z_sq*omega_k + omega_v );
            return 1.0/e_z;
         }

	 T omega_v, omega_m, omega_k;
      };

      ///
      /// Result is in Mpc.
      ///
      template< class T >
      T
      redshift_to_comoving_distance( T z,
				     unsigned points = 1000,
				     T hubble = 71.0,
				     T omega_v = 0.73,
				     T omega_m = 0.27 )
      {
         ASSERT( z >= 0.0 );
         T sum = simpson( redshift_to_comoving_distance_func<T>( omega_v, omega_m ), 0, z, points );
         return (constant::c_km_s/hubble)*sum;
      }

      template< class T >
      struct redshift_to_light_travel_distance_func
      {
         typedef T value_type;

         redshift_to_light_travel_distance_func( T omega_v = 0.73,
						 T omega_m = 0.27 )
            : omega_v( omega_v ),
	      omega_m( omega_m ),
              omega_k( 1.0 - omega_m - omega_v )
         {
         }

         T
         operator()( T z ) const
         {
	    double z_sq = (1.0 + z)*(1.0 + z);
	    double e_z = sqrt( z_sq*(1.0 + z)*omega_m + z_sq*omega_k + omega_v );
            return 1.0/((1.0 + z)*e_z);
         }

	 T omega_v, omega_m, omega_k;
      };

      ///
      /// Result is in Mpc.
      ///
      template< class T >
      T
      redshift_to_light_travel_distance( T z,
					 unsigned points = 1000,
					 T hubble = 71.0,
					 T omega_v = 0.73,
					 T omega_m = 0.27 )
      {
         ASSERT( z >= 0.0 );
         T sum = simpson( redshift_to_light_travel_distance_func<T>( omega_v, omega_m ), 0.0, z, points );
         return (constant::c_km_s/hubble)*sum;
      }

      ///
      /// Result is in Mpc.
      ///
      template< class T >
      T
      redshift_to_transverse_distance( T z,
				       unsigned points = 1000,
				       T hubble = 71.0,
				       T omega_v = 0.73,
				       T omega_m = 0.27 )
      {
         ASSERT( z >= 0.0 );
         T sum = simpson( redshift_to_comoving_distance_func<T>( omega_v, omega_m ), 0.0, z, points );
         T dh = constant::c_km_s/hubble;
	 sum *= dh;
	 T omega_k = 1.0 - omega_m - omega_v;
	 if( omega_k > 1e-8 )
	 {
	    sum = sinh( sqrt( omega_k )*sum/dh )*dh/sqrt( omega_k );
	 }
	 else if( omega_k < -1e-8 )
	 {
	    sum = sinh( sqrt( -omega_k )*sum/dh )*dh/sqrt( -omega_k );
	 }
         return sum;
      }

      ///
      /// Result is in Mpc.
      ///
      template< class T >
      T
      redshift_to_luminosity_distance( T z,
				       unsigned points = 1000,
				       T hubble = 71.0,
				       T omega_v = 0.73,
				       T omega_m = 0.27 )
      {
	 return (1.0 + z)*redshift_to_transverse_distance( z, points, hubble, omega_v, omega_m );
      }

      ///
      /// Result is in Mpc.
      ///
      template< class T >
      T
      redshift_to_angular_diameter_distance( T z,
                                             unsigned points = 1000,
                                             T hubble = 71.0,
                                             T omega_v = 0.73,
                                             T omega_m = 0.27 )
      {
	 return redshift_to_transverse_distance( z, points, hubble, omega_v, omega_m )/(1.0 + z);
      }
   }
}

#endif
