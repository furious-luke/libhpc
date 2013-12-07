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

#ifndef hpc_containers_hh
#define hpc_containers_hh

#include <limits>

namespace hpc {
   namespace num {

      template< class T >
      T
      approx( T x1,
              T x2,
              T delta = std::numeric_limits<T>::epsilon() )
      {
	 return (x1 > x2 - delta && x1 < x2 + delta);
      }

      template< class T >
      bool
      is_zero( T x )
      {
	 return approx( x, (T)0.0 );
      }

      template< class T >
      T
      distribute( T x,
                  T rank,
                  T num_ranks )
      {
         return x/num_ranks + ((rank < (x%num_ranks)) ? 1 : 0);
      }
   }
}

#endif
