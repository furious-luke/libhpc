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

#ifndef hpc_algorithm_newton_hh
#define hpc_algorithm_newton_hh

#include "libhpc/system/cc_version.hh"
#include "libhpc/system/cuda.hh"
#include "libhpc/system/math.hh"
#include "libhpc/system/array.hh"
#include "libhpc/debug/assert.hh"
#include "libhpc/debug/except.hh"
#include "libhpc/logging.hh"

namespace hpc {
   namespace alg {

#ifdef CXX_0X
      static constexpr double default_newton_tolerance = 1e-8;
      static constexpr unsigned default_newton_max_its = 60;
#else
#define default_newton_tolerance 1e-8
#define default_newton_max_its   60
#endif

      template< class FuncT,
		class T >
      CUDA_DEV_HOST
      T
      newton( FuncT& func,
              T const& x1,
              T const& x2,
              T x,
              T const& tol = default_newton_tolerance,
	      unsigned max_its = default_newton_max_its,
	      T omega = 1.0 )
      {
	 // LOGBLOCKD_TAG( "newton", "Newton-Raphson solve:" );

         // Use a scaling to make sure we don't converge simply
         // because x is very small.
         T scale;
         if( x > 0.0 )
            scale = 1.0/x;
         else
            scale = 1.0;

         // Only loop up until or maximum.
         T f, df, delta, new_x;
         unsigned ii;
         for( ii = 0; ii < max_its; ++ii )
         {
	    LOGBLOCKD( "Iteration: ", ii );

            f = func( x );
            df = func.deriv( x, f );
            delta = f/df;
            LOGDLN( "x        = ", x );
	    LOGDLN( "f(x)     = ", f );
	    LOGDLN( "df/dx    = ", df );
	    LOGDLN( "delta x  = ", delta );
	    LOGDLN( "delta x% = ", delta*scale );
	    LOGDLN( "old x    = ", x );
            new_x = (1.0 - omega)*x + omega*(x - delta);
	    LOGDLN( "new x    = ", x );

            // Values must remain bracketed.
            ASSERT( (x1 - new_x)*(new_x - x2) >= 0.0,
                    "Function moved outside bracketed range in Newton's method." );

            // Check for convergence. Note that I use "x" instead of
            // "new_x" to be sure we check for percentage difference.
            if( hpc::fabs( delta*scale ) < tol )
            {
               x = new_x;
               break;
            }

            // Update x value.
            x = new_x;
         }

         // Break if we exceeded maximum its.
         EXCEPT( ii < max_its, "Newton iterations exceeded limit." );

         return x;
      }

      template< class FuncT,
		class T >
      CUDA_DEV_HOST
      hpc::array<T,2>
      newtond( FuncT& func,
               T const& x1,
               T const& x2,
               T x,
               T const& tol = default_newton_tolerance,
               unsigned max_its = default_newton_max_its,
               T omega = 1.0 )
      {
	 // LOGBLOCKD_TAG( "newton", "Newton-Raphson solve:" );

         // Use a scaling to make sure we don't converge simply
         // because x is very small.
         T scale;
         if( x > 0.0 )
            scale = 1.0/x;
         else
            scale = 1.0;

         // Only loop up until or maximum.
         T f, df, delta, new_x;
         unsigned ii;
         for( ii = 0; ii < max_its; ++ii )
         {
	    LOGBLOCKD( "Iteration: ", ii );

            f = func( x );
            df = func.deriv( x, f );
            delta = f/df;
            LOGDLN( "x        = ", x );
	    LOGDLN( "f(x)     = ", f );
	    LOGDLN( "df/dx    = ", df );
	    LOGDLN( "delta x  = ", delta );
	    LOGDLN( "delta x% = ", delta*scale );
	    LOGDLN( "old x    = ", x );
            new_x = (1.0 - omega)*x + omega*(x - delta);
	    LOGDLN( "new x    = ", x );

            // Values must remain bracketed.
            ASSERT( (x1 - new_x)*(new_x - x2) >= 0.0,
                    "Function moved outside bracketed range in Newton's method." );

            // Check for convergence. Note that I use "x" instead of
            // "new_x" to be sure we check for percentage difference.
            if( hpc::fabs( delta*scale ) < tol )
            {
               x = new_x;
               break;
            }

            // Update x value.
            x = new_x;
         }

         // Break if we exceeded maximum its.
         EXCEPT( ii < max_its, "Newton iterations exceeded limit." );

#ifdef CXX_0X
         return hpc::array<T,2>{ x, df };
#else
	 hpc::array<T,2> res;
	 res[0] = x;
	 res[1] = df;
	 return res;
#endif
      }

      // template< class Function,
      //   	class T >
      // bool
      // newton_test( Function& func,
      //   	   T x1,
      //   	   T x2,
      //   	   T& result,
      //   	   optional<T> initial = optional<T>(),
      //   	   optional<T&> deriv = optional<T&>(),
      //   	   T tolerance = default_newton_tolerance,
      //   	   unsigned max_its = default_newton_max_its,
      //   	   T omega = 1.0 )
      // {
      //    LOGBLOCKD_TAG( "newton", "Newton-Raphson solve:" );

      //    // Were we given an initial value? If not, set it to the midpoint.
      //    T x = initial ? *initial : 0.5*(x2 + x1);
      //    T init = x;
      //    LOGDLN( "Starting with x: ", x );

      //    // Only loop up until or maximum.
      //    T f, df, delta;
      //    int ii;
      //    for( ii = 0; ii < max_its; ++ii )
      //    {
      //       LOGBLOCKD( "Iteration: ", ii );

      //       f = func( x );
      //       df = func.derivative( x, f );
      //       delta = f/df;
      //       LOGDLN( "f(", x, ") = ", f );
      //       LOGDLN( "df/dx = ", df );
      //       LOGDLN( "delta x = ", delta );
      //       LOGDLN( "old x = ", x );
      //       x = (1.0 - omega)*x + omega*(x - delta);
      //       LOGDLN( "new x = ", x );

      //       // Values must remain bracketed.
      //       if( (x1 - x)*(x - x2) < 0.0 )
      //       {
      //          result = init;
      //          return false;
      //       }

      //       // Check for convergence.
      //       if( fabs( delta ) < tolerance )
      //          break;
      //    }

      //    // Store result.
      //    result = x;

      //    // Store function derivative?
      //    if( deriv )
      //       *deriv = df;

      //    // Break if we exceeded maximum its.
      //    if( ii == max_its )
      //       return false;

      //    return true;
      // }

      // template< class Function,
      //   	class T >
      // T
      // newton_richardson( Function& func,
      //                    T x1,
      //                    T x2,
      //                    T init = std::numeric_limits<T>::max(),
      //                    T tol = default_newton_tolerance,
      //                    unsigned max_its = default_newton_max_its,
      //                    T omega = 1.0 )
      // {
      //    LOGBLOCKD_TAG( "newton", "Newton-Raphson bisect solve:" );

      //    // Were we given an initial value? If not, set it to the midpoint.
      //    T x = (init != std::numeric_limits<T>::max()) ? init : 0.5*(x2 + x1);
      //    LOGDLN( "Starting with x: ", x );

      //    // Only loop up until or maximum.
      //    T f, df, delta, old_delta = std::numeric_limits<T>::max();
      //    int ii;
      //    for( ii = 0; ii < max_its; ++ii )
      //    {
      //       LOGBLOCKD( "Iteration: ", ii );

      //       f = func( x );
      //       df = func.derivative( x, f );
      //       delta = f/df;
      //       LOGDLN( "f(", x, ") = ", f );
      //       LOGDLN( "df/dx = ", df );
      //       LOGDLN( "delta x = ", delta );
      //       LOGDLN( "old x = ", x );
      //       T new_x = (1.0 - omega)*x + omega*(x - delta);
      //       LOGDLN( "new x = ", new_x );

      //       // Values must remain bracketed.
      //       ASSERT( (x1 - new_x)*(new_x - x2) >= 0.0,
      //               "Function moved outside bracketed range in Newton's method." );

      //       // Check for convergence.
      //       T mag_delta = fabs( delta );
      //       if( mag_delta < tol )
      //          break;

      //       // Check for problems.
      //       if( mag_delta >= old_delta )
      //       {
      //          // Take Richardson value.
      //          new_x = func.richardson( x, f );
      //          LOGDLN( "Richardson: ", new_x );
      //       }
      //       else
      //          old_delta = mag_delta;

      //       // Update values.
      //       x = new_x;
      //    }

      //    // Break if we exceeded maximum its.
      //    EXCEPT( ii < max_its );

      //    return x;
      // }

      // template< class Function,
      //   	class T >
      // std::tuple<T,unsigned>
      // newton_its( Function& func,
      //             T x1,
      //             T x2,
      //             T init = std::numeric_limits<T>::max(),
      //             T tol = default_newton_tolerance,
      //             unsigned max_its = default_newton_max_its,
      //             T omega = 1.0 )
      // {
      //    LOGBLOCKD_TAG( "newton", "Newton-Raphson bisect solve:" );

      //    // Were we given an initial value? If not, set it to the midpoint.
      //    T x = (init != std::numeric_limits<T>::max()) ? init : 0.5*(x2 + x1);
      //    LOGDLN( "Starting with x: ", x );

      //    // Only loop up until or maximum.
      //    T f, df, delta;
      //    unsigned ii;
      //    for( ii = 0; ii < max_its; ++ii )
      //    {
      //       LOGBLOCKD( "Iteration: ", ii );

      //       f = func( x );
      //       df = func.derivative( x, f );
      //       delta = f/df;
      //       LOGDLN( "f(", x, ") = ", f );
      //       LOGDLN( "df/dx = ", df );
      //       LOGDLN( "delta x = ", delta );
      //       LOGDLN( "old x = ", x );
      //       x = (1.0 - omega)*x + omega*(x - delta);
      //       LOGDLN( "new x = ", x );

      //       // Values must remain bracketed.
      //       ASSERT( (x1 - x)*(x - x2) >= 0.0,
      //               "Function moved outside bracketed range in Newton's method." );

      //       // Check for convergence.
      //       if( fabs( delta ) < tol )
      //          break;
      //    }

      //    // Break if we exceeded maximum its.
      //    EXCEPT( ii < max_its );

      //    return std::make_tuple( x, ii );
      // }

   }
}

#endif
