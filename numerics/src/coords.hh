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
         ASSERT( !num::approx<T>( x, 0 ) );
         r = sqrt( x*x + y*y + z*z );
         ASSERT( !num::approx<T>( r, 0 ) );
         phi = atan( y/x );
         theta = acos( z/r );
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

      template< class T >
      void
      gnomonic_projection( T object_asc, T object_dec,
                           T origin_asc, T origin_dec,
                           T& x, T& y )
      {
         T sind = sin( object_dec ), cosd = cos( object_dec );
         T sind0 = sin( origin_dec ), cosd0 = cos( origin_dec );
         T cosa = cos( object_asc - origin_asc );
         x = -cosd*sin( object_asc - origin_asc )/(cosd0*cosd*cosa + sind*sind0);
         y = -(sind0*cosd*cosa - cosd0*sind)/(cosd0*cosd*cosa + sind*sind0);
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
