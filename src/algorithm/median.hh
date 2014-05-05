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

#ifndef hpc_algorithm_median_hh
#define hpc_algorithm_median_hh

#include "libhpc/debug/assert.hh"
#include "libhpc/mpi/comm.hh"
#include "select.hh"

namespace hpc {
   namespace algorithm {

      template< class Iterator >
      typename Iterator::value_type
      median( Iterator const& start,
              Iterator const& finish,
              mpi::comm const& comm = mpi::comm::world )
      {
         typedef typename Iterator::value_type value_type;

         // Find the position of the median.
         long pos = comm.all_reduce( std::distance( start, finish ), MPI_SUM );
         pos >>= 1;

         return select( start, finish, pos, comm );
      }

   }
}

#endif
