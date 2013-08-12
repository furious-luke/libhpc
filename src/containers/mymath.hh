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

int
powi( int x,
      int e );

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
   return pow2i(log2i(x));
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
   return (T(0) < x) - (x < T(0));
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

void
tri_bary( const double** crds,
	  const double* pnt,
	  double* bc );

#endif
