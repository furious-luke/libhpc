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

namespace hpc {

   ///
   /// Spherical coordinates in common physics interpretation, theta being
   /// the angle between the z axis and r. Phi being the angle between the
   /// x axis and the y axis.
   ///
   /// theta = inclination
   /// phi = azimuth
   ///
   template< class T >
   cartesian_to_spherical( T x, T y, T z,
                           T& r, T& theta, T& phi )
   {
      ASSERT( !num::approx<T>( x, 0 ) );
      r = sqrt( x*x + y*y + z*z );
      ASSERT( !num::approx<T>( r, 0 ) );
      phi = atan( y/x );
      theta = acos( z/r );
   }
}

#endif
