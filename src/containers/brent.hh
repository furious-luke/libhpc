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

#ifndef libhpc_containers_brent_hh
#define libhpc_containers_brent_hh

#include <boost/optional/optional.hpp>
#include "libhpc/debug/assert.hh"
#include "libhpc/debug/except.hh"
#include "libhpc/logging/logging.hh"
#include "num.hh"

namespace hpc {

   static constexpr double default_brent_tolerance = 1e-8;
   static constexpr unsigned default_brent_max_its = 60;

   template< class Function,
	     class T >
   T
   brent( Function& func,
	  T x1,
	  T x2,
	  boost::optional<T> initial = boost::optional<T>(),
	  T tol = default_brent_tolerance,
	  unsigned max_its = default_brent_max_its )
   {
      LOGBLOCKT( "Brent-Drekker solve:" );

      // // Were we given an initial value? If not, set it to the midpoint.
      // T x = initial ? *initial : 0.5*(x2 + x1);
      // LOGTLN( "Starting with x: ", x );

      double a = x1;
      double b = x2;
      double c = 0.0;
      double d = std::numeric_limits<double>::max();
 
      double fa = func( a );
      double fb = func( b );
 
      double fc = 0.0;
      double s = 0.0;
      double fs = 0.0;
 
      // if f(a) f(b) >= 0 then error-exit
      if( fa*fb >= 0.0 )
      {
	 if( fa < fb )
	    return a;
	 else
	    return b;
      }
 
      // if |f(a)| < |f(b)| then swap (a,b) end if
      if( fabs( fa ) < fabs( fb ) )
      {
	 std::swap( a, b );
	 std::swap( fa, fb );
      }

      c = a;
      fc = fa;
      bool mflag = true;
      int ii = 0;
 
      while( !(fb == 0) && (fabs( a - b ) > tol) )
      {
	 if( (fa != fc) && (fb != fc) )
	 {
	    // Inverse quadratic interpolation
	    s = a * fb * fc / (fa - fb) / (fa - fc) + b * fa * fc / (fb - fa) / (fb - fc) + c * fa * fb / (fc - fa) / (fc - fb);
	 }
	 else
	 {
	    // Secant Rule
	    s = b - fb * (b - a) / (fb - fa);
	 }
 
	 double tmp2 = (3.0 * a + b) / 4.0;
	 if ((!(((s > tmp2) && (s < b)) || ((s < tmp2) && (s > b)))) || (mflag && (fabs(s - b) >= (fabs(b - c) / 2.0))) || (!mflag && (fabs(s - b) >= (fabs(c - d) / 2.0))))
	 {
	    s = (a + b) / 2;
	    mflag = true;
	 }
	 else
	 {
	    if ((mflag && (fabs(b - c) < tol)) || (!mflag && (fabs(c - d) < tol)))
	    {
	       s = (a + b) / 2.0;
	       mflag = true;
	    }
	    else
	       mflag = false;
	 }
	 fs = func( s );
	 d = c;
	 c = b;
	 fc = fb;
	 if (fa * fs < 0.0) { b = s; fb = fs; }
	 else { a = s; fa = fs; }
 
	 // if |f(a)| < |f(b)| then swap (a,b) end if
	 if (fabs(fa) < fabs(fb))
	 {
	    std::swap( a, b );
	    std::swap( fa, fb );
	 }
	 ++ii;
	 LOGTLN( "x = ", b );
	 EXCEPT( ii < max_its, "Too many Brent-Drekker iterations." );
      }
      return b;
   }

}

#endif
