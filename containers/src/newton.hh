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
