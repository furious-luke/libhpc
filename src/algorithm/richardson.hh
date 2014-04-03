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

#ifndef libhpc_containers_richardson_hh
#define libhpc_containers_richardson_hh

#include <limits>
#include "libhpc/debug/assert.hh"
#include "libhpc/debug/except.hh"
#include "libhpc/logging/logging.hh"

namespace hpc {

   static constexpr double default_richardson_tolerance = 1e-5;
   static constexpr unsigned default_richardson_max_its = 60;

   template< class Function,
             class T >
   std::tuple<T,unsigned>
   richardson( Function& func,
               T x1,
               T x2,
               T init = std::numeric_limits<T>::max(),
               T tol = default_richardson_tolerance,
               unsigned max_its = default_richardson_max_its,
               T omega = 1.0 )
   {
      LOGBLOCKT_TAG( "richardson", "Richardson solve:" );

      // Were we given an initial value? If not, set it to the midpoint.
      T x = (init != std::numeric_limits<T>::max()) ? init : 0.5*(x2 + x1);
      LOGTLN( "Starting with x: ", x );

      // Only loop up until or maximum.
      T f, new_x;
      int ii;
      for( ii = 0; ii < max_its; ++ii )
      {
         LOGBLOCKT( "Iteration: ", ii );
         f = func( x );
         LOGTLN( "f(", x, ") = ", f );
         new_x = (1.0 - omega)*x + omega*func.richardson( x, f );
         LOGTLN( "new x = ", new_x );

         // Check for convergence.
         if( fabs( x - new_x ) < tol )
            break;

         // Update values.
         x = new_x;
      }

      // Break if we exceeded maximum its.
      EXCEPT( ii < max_its, "Richardson solve required too many iterations." );

      return std::make_tuple( new_x, ii );
   }

}

#endif
