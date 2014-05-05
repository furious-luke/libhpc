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

#ifndef hpc_numerics_generators_hh
#define hpc_numerics_generators_hh

#include <math.h>
#include "libhpc/debug/assert.hh"

namespace hpc {
   namespace num {

      template< class Iter >
      void
      gaussian( Iter start,
                unsigned num_points,
                typename Iter::value_type amp = 1 )
      {
         typedef typename Iter::value_type value_type;
         for( unsigned ii = 0; ii < num_points; ++ii )
         {
            value_type x = ((value_type)ii/(value_type)(num_points - 1))*(value_type)2 - (value_type)1;
            *start++ = amp*exp( -(x*x) );
         }
      }

      template< class Iter >
      void
      linear( Iter start,
              unsigned size,
              typename Iter::value_type min = 0,
              typename Iter::value_type max = 1 )
      {
         if( size )
         {
            typename Iter::value_type step = (max - min)/(size - 1);
            for( unsigned ii = 0; ii < size; ++start, ++ii )
               *start = min + ii*step;
         }
      }

   }
}

#endif
