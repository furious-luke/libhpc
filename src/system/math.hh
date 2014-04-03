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

#ifndef containers_mymath_hh
#define containers_mymath_hh

#include <math.h>
#include "libhpc/debug/debug.hh"

namespace hpc {

   template< class T >
   T
   approx( T x1,
           T x2,
           T delta = std::numeric_limits<T>::epsilon() )
   {
      return (x1 > x2 - delta && x1 < x2 + delta);
   }

   template< class T >
   bool
   is_zero( T x )
   {
      return approx( x, (T)0.0 );
   }

   template< class T >
   T
   log2i( T x )
   {
      T e = 0;
      ASSERT(x > 0);
      while((x >> e) != 1)
         e++;
      return e;
   }

   template< class T >
   T
   powi( T x,
         T e )
   {
      T r = 1;
      ASSERT( x >= 0 );
      while( e )
      {
         r *= x;
         e--;
      }
      return r;
   }

   template< class T >
   T
   pow2i( T e )
   {
      return 1 << e;
   }

   template< class T >
   T
   nearest_po2( T x )
   {
      return pow2i( log2i( x ) );
   }

   template< class T >
   T
   make_po2( T x )
   {
      T l = log2i( x );
      T p = pow2i( l );
      if( p == x )
         return x;
      else
         return pow2i( l + 1 );
   }

   template< class T >
   int
   sgn( T x )
   {
      return (T( 0 ) < x) - (x < T( 0 ));
   }

   template< class T >
   T
   to_radians( T x )
   {
      return x*(M_PI/180.0);
   }

   template< class T >
   T
   to_degrees( T x )
   {
      return x*(180.0/M_PI);
   }

   template< class T >
   T
   distribute( T x,
               T rank,
               T num_ranks )
   {
      return x/num_ranks + ((rank < (x%num_ranks)) ? 1 : 0);
   }

}

#endif
