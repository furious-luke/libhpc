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
#include "libhpc/debug/except.hh"
#include "libhpc/logging.hh"

using boost::optional;

namespace hpc {
   namespace num {

      static constexpr double default_newton_tolerance = 1e-8;
      static constexpr unsigned default_newton_max_its = 60;

      template< class Function,
		class T >
      T
      newton( Function& func,
              T x1,
              T x2,
              optional<T> initial = optional<T>(),
              optional<T&> deriv = optional<T&>(),
              T tolerance = default_newton_tolerance,
	      unsigned max_its = default_newton_max_its,
	      T omega = 1.0 )
      {
	 LOGBLOCKT_TAG( "newton", "Newton-Raphson solve:" );

         // Were we given an initial value? If not, set it to the midpoint.
         T x = initial ? *initial : 0.5*(x2 + x1);
	 LOGTLN( "Starting with x: ", x );

         // Only loop up until or maximum.
         T f, df, delta;
         int ii;
         for( ii = 0; ii < max_its; ++ii )
         {
	    LOGBLOCKT( "Iteration: ", ii );

            f = func( x );
            df = func.derivative( x, f );
            delta = f/df;
	    LOGTLN( "f(", x, ") = ", f );
	    LOGTLN( "df/dx = ", df );
	    LOGTLN( "delta x = ", delta );
	    LOGTLN( "old x = ", x );
            x = (1.0 - omega)*x + omega*(x - delta);
	    LOGTLN( "new x = ", x );

            // Values must remain bracketed.
            ASSERT( (x1 - x)*(x - x2) >= 0.0,
                    "Function moved outside bracketed range in Newton's method." );

            // Check for convergence.
            if( fabs( delta ) < tolerance )
               break;
         }

         // Break if we exceeded maximum its.
         EXCEPT( ii < max_its );

         // Store function derivative?
         if( deriv )
            *deriv = df;

         return x;
      }

      template< class Function,
		class T >
      bool
      newton_test( Function& func,
		   T x1,
		   T x2,
		   T& result,
		   optional<T> initial = optional<T>(),
		   optional<T&> deriv = optional<T&>(),
		   T tolerance = default_newton_tolerance,
		   unsigned max_its = default_newton_max_its,
		   T omega = 1.0 )
      {
	 LOGBLOCKT_TAG( "newton", "Newton-Raphson solve:" );

         // Were we given an initial value? If not, set it to the midpoint.
         T x = initial ? *initial : 0.5*(x2 + x1);
	 T init = x;
	 LOGTLN( "Starting with x: ", x );

         // Only loop up until or maximum.
         T f, df, delta;
         int ii;
         for( ii = 0; ii < max_its; ++ii )
         {
	    LOGBLOCKT( "Iteration: ", ii );

            f = func( x );
            df = func.derivative( x, f );
            delta = f/df;
	    LOGTLN( "f(", x, ") = ", f );
	    LOGTLN( "df/dx = ", df );
	    LOGTLN( "delta x = ", delta );
	    LOGTLN( "old x = ", x );
            x = (1.0 - omega)*x + omega*(x - delta);
	    LOGTLN( "new x = ", x );

            // Values must remain bracketed.
            if( (x1 - x)*(x - x2) < 0.0 )
	    {
	       result = init;
	       return false;
	    }

            // Check for convergence.
            if( fabs( delta ) < tolerance )
               break;
         }

	 // Store result.
	 result = x;

         // Store function derivative?
         if( deriv )
            *deriv = df;

         // Break if we exceeded maximum its.
         if( ii == max_its )
	    return false;

         return true;
      }

      template< class Function,
		class T >
      T
      newton_richardson( Function& func,
                         T x1,
                         T x2,
                         T init = std::numeric_limits<T>::max(),
                         T tol = default_newton_tolerance,
                         unsigned max_its = default_newton_max_its,
                         T omega = 1.0 )
      {
	 LOGBLOCKT_TAG( "newton", "Newton-Raphson bisect solve:" );

         // Were we given an initial value? If not, set it to the midpoint.
         T x = (init != std::numeric_limits<T>::max()) ? init : 0.5*(x2 + x1);
	 LOGTLN( "Starting with x: ", x );

         // Only loop up until or maximum.
         T f, df, delta, old_delta = std::numeric_limits<T>::max();
         int ii;
         for( ii = 0; ii < max_its; ++ii )
         {
	    LOGBLOCKT( "Iteration: ", ii );

            f = func( x );
            df = func.derivative( x, f );
            delta = f/df;
	    LOGTLN( "f(", x, ") = ", f );
	    LOGTLN( "df/dx = ", df );
	    LOGTLN( "delta x = ", delta );
	    LOGTLN( "old x = ", x );
            T new_x = (1.0 - omega)*x + omega*(x - delta);
	    LOGTLN( "new x = ", new_x );

            // Values must remain bracketed.
            ASSERT( (x1 - new_x)*(new_x - x2) >= 0.0,
                    "Function moved outside bracketed range in Newton's method." );

            // Check for convergence.
            T mag_delta = fabs( delta );
            if( mag_delta < tol )
               break;

            // Check for problems.
            if( mag_delta >= old_delta )
            {
               // Take Richardson value.
               new_x = func.richardson( x, f );
               LOGDLN( "Richardson: ", new_x );
            }
            else
               old_delta = mag_delta;

            // Update values.
            x = new_x;
         }

         // Break if we exceeded maximum its.
         EXCEPT( ii < max_its );

         return x;
      }

      template< class Function,
		class T >
      std::tuple<T,unsigned>
      newton_its( Function& func,
                  T x1,
                  T x2,
                  T init = std::numeric_limits<T>::max(),
                  T tol = default_newton_tolerance,
                  unsigned max_its = default_newton_max_its,
                  T omega = 1.0 )
      {
	 LOGBLOCKT_TAG( "newton", "Newton-Raphson bisect solve:" );

         // Were we given an initial value? If not, set it to the midpoint.
         T x = (init != std::numeric_limits<T>::max()) ? init : 0.5*(x2 + x1);
	 LOGTLN( "Starting with x: ", x );

         // Only loop up until or maximum.
         T f, df, delta;
         unsigned ii;
         for( ii = 0; ii < max_its; ++ii )
         {
	    LOGBLOCKT( "Iteration: ", ii );

            f = func( x );
            df = func.derivative( x, f );
            delta = f/df;
	    LOGTLN( "f(", x, ") = ", f );
	    LOGTLN( "df/dx = ", df );
	    LOGTLN( "delta x = ", delta );
	    LOGTLN( "old x = ", x );
            x = (1.0 - omega)*x + omega*(x - delta);
	    LOGTLN( "new x = ", x );

            // Values must remain bracketed.
            ASSERT( (x1 - x)*(x - x2) >= 0.0,
                    "Function moved outside bracketed range in Newton's method." );

            // Check for convergence.
            if( fabs( delta ) < tol )
               break;
         }

         // Break if we exceeded maximum its.
         EXCEPT( ii < max_its );

         return std::make_tuple( x, ii );
      }

   }
}

#endif
