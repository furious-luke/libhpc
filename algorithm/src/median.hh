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

#ifndef hpc_algorithm_select_hh
#define hpc_algorithm_select_hh

#include "libhpc/debug/assert.hh"
#include "libhpc/containers/num.hh"
#include "libhpc/containers/optional.hh"
#include "libhpc/containers/mymath.hh"

namespace hpc {
   namespace algorithm {

      template< class Iterator >
      typename Iterator::value_type
      median( const Iterator& start,
              const Iterator& finish,
              const mpi::comm& comm = mpi::comm::world )
      {
         typedef typename Iterator::value_type value_type;

         // Find the position of the median.
         long position = comm.all_reduce( std::distance( start, finish ), MPI_SUM );
         position >>= 1;

         return select( start, finish, position, comm );
      }

   }
}

#endif
