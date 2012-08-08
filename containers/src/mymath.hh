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
