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
#include "constants.hh"

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
      /// Spherical coordinates in common physics interpretation, theta being
      /// the angle between the z axis and r. Phi being the angle between the
      /// x axis and the y axis.
      ///
      /// theta = inclination
      /// phi = azimuth
      ///
      template< class T >
      void
      cartesian_to_ecs( T x, T y, T z,
                        T& ra, T& dec )
      {
         T r;
         cartesian_to_spherical( x, y, z, r, dec, ra );
         dec = 0.5*M_PI - dec;
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
                        T& x, T& y, T& z )
                        
      {
         dec = 0.5*M_PI - dec;
         spherical_to_cartesian( 1.0, dec, ra, x, y, z );
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
      struct redshift_to_distance_func
      {
         typedef T value_type;

         redshift_to_distance_func( T omega_l=0.73 )
            : omega_l( omega_l ),
              omega_m( 1.0 - omega_l )
         {
         }

         T
         operator()( T z ) const
         {
            return 1.0/sqrt( (1.0 + z)*(1.0 + z)*(1.0 + omega_m*z) - z*omega_l*(2.0 + z) );
         }

         T omega_l, omega_m;
      };

      ///
      ///
      ///
      template< class T >
      T
      redshift_to_distance( T z,
                            unsigned points,
                            T hubble=70.0,   // km/s/Mpc
                            T omega_l=0.73 ) // km/s/Mpc
      {
         ASSERT( z > 0.0 );
         T sum = simpson( redshift_to_distance_func<T>( omega_l ), 0.0, z, points );
         return sum*(1.0 + z)*constant::c/hubble;
      }
   }
}

#endif
