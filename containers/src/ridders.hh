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

#ifndef hpc_containers_ridders_hh
#define hpc_containers_ridders_hh

#include <boost/optional/optional.hpp>
#include "libhpc/debug/assert.hh"
#include "num.hh"

namespace hpc {
   namespace num {

      static const double default_xtol = 1e-8;

      template< class T >
      int
      sign( T x )
      {
	 return (x > T(0)) - (x < T(0));
      }

      template< class Function,
		class T >
      T
      ridders( Function func,
	       T x1,
	       T x2,
	       T xtol=default_xtol,
	       boost::optional<unsigned&> num_its=boost::optional<unsigned&>() )
      {
	 static const unsigned max_its = 60;

	 T f1, f2;
	 f1 = func(x1);
	 f2 = func(x2);

	 if(f1 > 0.0 && f2 < 0.0 || f1 < 0.0 && f2 > 0.0) {
	    T x3, x4, xn, f3, f4, denom;
	    x4 = std::numeric_limits<double>::max();
	    unsigned ii;
	    for(ii = 0; ii < max_its; ++ii) {

	       x3 = 0.5*(x1 + x2);
	       f3 = func(x3);
	       denom = sqrt(f3*f3 - f1*f2);
	       if(num::is_zero(denom))
		  break;

	       xn = x3 + (x3 - x1)*((double)sign(f1 - f2)*f3)/denom;
	       if(fabs(x4 - xn) <= xtol)
		  break;

	       x4 = xn;
	       f4 = func(x4);
	       if(sign(f4) != sign(f3)) {
		  x1 = x3;
		  f1 = f3;
		  x2 = x4;
		  f2 = f4;
	       }
	       else if(sign(f4) != sign(f1)) {
		  x2 = x4;
		  f2 = f4;
	       }
	       else {
		  ASSERT(sign(f4) != sign(f2));
		  x1 = x4;
		  f1 = f4;
	       }

	       if(fabs(x2 - x1) <= xtol)
		  break;
	    }
	    ASSERT(ii < 60, "Failed to converge.");

	    if(num_its)
	       *num_its = ii;
	    return x4;
	 }
	 else if(num::is_zero(f1))
	    return x1;
	 else {
	    ASSERT(num::is_zero(f2), "Initial x values must bracket root.");
	    return x2;
	 }
      }

   }
}

#endif
