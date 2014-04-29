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

#ifndef hpc_algorithm_ridders_hh
#define hpc_algorithm_ridders_hh

#include <boost/optional.hpp>
#include "libhpc/debug/assert.hh"
#include "libhpc/system/math.hh"

namespace hpc {
   namespace algorithm {

      static const double default_xtol = 1e-8;
      static const unsigned default_max_its = 60;

      template< class Function,
		class T >
      boost::optional<T>
      ridders( Function func,
	       T x1,
	       T x2,
	       boost::optional<unsigned&> num_its = boost::optional<unsigned&>(),
               T xtol = default_xtol,
               unsigned max_its = default_max_its )
      {
         // Evaluate boundaries.
	 T f1, f2;
	 f1 = func( x1 );
	 f2 = func( x2 );

         // Check that the zero is bounded.
	 if( f1 > 0.0 && f2 < 0.0 || f1 < 0.0 && f2 > 0.0 )
         {
            // Repeat at most max_its times.
	    T x3, x4, xn, f3, f4, denom;
	    x4 = std::numeric_limits<double>::max();
	    unsigned ii;
	    for( ii = 0; ii < max_its; ++ii )
            {
	       x3 = 0.5*(x1 + x2);
	       f3 = func( x3 );
	       denom = sqrt( f3*f3 - f1*f2 );
	       if( is_zero( denom ) )
		  break;

	       xn = x3 + (x3 - x1)*((double)sgn( f1 - f2 )*f3)/denom;
	       if(fabs(x4 - xn) <= xtol)
		  break;

	       x4 = xn;
	       f4 = func( x4 );
	       if( sgn( f4 ) != sgn( f3 ) )
               {
		  x1 = x3;
		  f1 = f3;
		  x2 = x4;
		  f2 = f4;
	       }
	       else if( sgn( f4 ) != sgn( f1 ) )
               {
		  x2 = x4;
		  f2 = f4;
	       }
	       else
               {
		  ASSERT( sgn( f4 ) != sgn( f2 ) );
		  x1 = x4;
		  f1 = f4;
	       }

	       if( fabs( x2 - x1 ) <= xtol )
		  break;
	    }
	    ASSERT( ii < 60, "Failed to converge." );

	    if( num_its )
	       *num_its = ii;
	    return x4;
	 }
	 else if( is_zero( f1 ) )
	    return x1;
	 else if( is_zero( f2 ) )
	    return x2;
	 else
	    return boost::none;
      }

   }
}

#endif
