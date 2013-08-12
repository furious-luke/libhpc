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

#ifndef hpc_containers_newton_hh
#define hpc_containers_newton_hh

#include <boost/optional/optional.hpp>
#include "libhpc/debug/assert.hh"
#include "num.hh"

using boost::optional;

namespace hpc {
   namespace num {

      static const double default_newton_tolerance = 1e-8;

      template< class Function,
		class T >
      T
      newton( Function& func,
              T x1,
              T x2,
              optional<T> initial = optional<T>(),
              optional<T&> deriv = optional<T&>(),
              T tolerance = default_newton_tolerance )
      {
	 static const unsigned max_its = 60;

         // Were we given an initial value? If not, set it to the midpoint.
         T x = initial ? *initial : 0.5*(x2 + x1);

         // Only loop up until or maximum.
         T f, df, delta;
         int ii;
         for( ii = 0; ii < max_its; ++ii ) {
            f = func( x );
            df = func.derivative( x, f );
            delta = f/df;
            x -= delta;

            // Values must remain bracketed.
            ASSERT(
               (x1 - x)*(x - x2) >= 0.0,
               "Function moved outside bracketed range in Newton's method."
               );

            // Check for convergence.
            if( fabs( delta ) < tolerance )
               break;
         }

         // Break if we exceeded maximum its.
         ASSERT( ii < max_its );

         // Store function derivative?
         if( deriv )
            *deriv = df;

         return x;
      }
   }
}

#endif
