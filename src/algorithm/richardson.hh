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

#ifndef hpc_algorithm_richardson_hh
#define hpc_algorithm_richardson_hh

#include "libhpc/system/cc_version.hh"
#include "libhpc/system/cuda.hh"
#include "libhpc/debug/assert.hh"
#include "libhpc/debug/except.hh"
#include "libhpc/logging.hh"

namespace hpc {

#ifdef CXX_0X
   static constexpr double default_richardson_tolerance = 1e-5;
   static constexpr unsigned default_richardson_max_its = 60;
#else
#define default_richardson_tolerance 1e-5
#define default_richardson_max_its   60
#endif

   template< class FuncT,
             class T >
   CUDA_DEV_HOST
   T
   richardson( FuncT func,
               T const& x1,
               T const& x2,
               T x,
               T const& tol = default_richardson_tolerance,
               unsigned max_its = default_richardson_max_its,
               T omega = 1.0 )
   {
      // LOGBLOCKD_TAG( "richardson", "Richardson solve:" );

      // Only loop up until or maximum.
      T f, new_x;
      unsigned ii;
      for( ii = 0; ii < max_its; ++ii )
      {
         LOGBLOCKD( "Iteration: ", ii );
         LOGDLN( "x     = ", x );
         f = func( x );
         LOGDLN( "f(x)  = ", f );
         new_x = (1.0 - omega)*x + omega*f;
         LOGDLN( "new x = ", new_x );

         // Values must remain bracketed.
         ASSERT( (x1 - x)*(x - x2) >= 0.0,
                 "Function moved outside bracketed range." );

         // Check for convergence.
         if( fabs( x - new_x ) < tol )
            break;

         // Update value.
         x = new_x;
      }

      // Break if we exceeded maximum its.
      EXCEPT( ii < max_its, "Richardson solve required too many iterations." );

      return new_x;
   }

}

#endif
