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
#include "libhpc/hpcmpi/comm.hh"
#include "ridders.hh"

namespace hpc {
   namespace algorithm {

      template< class Iterator >
      struct select_function
      {
         typedef typename Iterator::value_type value_type;

         select_function( const Iterator& start,
                          const Iterator& finish,
                          long position,
                          const mpi::comm& comm )
            : start( start ),
              finish( finish ),
              position( position ),
              comm( comm )
         {
         }

         long
         operator()( const value_type& x )
         {
            long sum_left = std::count_if(
               start, finish,
               std::bind2nd(
                  std::less<value_type>(), x
                  )
               );
            return comm.all_reduce( sum_left ) - position;
         }

         const Iterator start, finish;
         long position;
         const mpi::comm& comm;
      };

      template< class Iterator >
      typename Iterator::value_type
      select( const Iterator& start,
              const Iterator& finish,
              long position,
              const mpi::comm& comm = mpi::comm::world )
      {
         typedef typename Iterator::value_type value_type;

         ASSERT( position >= 0, "Invalid selection position." );

         // Find the minimum and maximum values.
         auto minmax = std::minmax_element( start, finish );
         value_type x1 = comm.all_reduce( *minmax.first, MPI_MIN );
         value_type x2 = comm.all_reduce( *minmax.second, MPI_MAX );

         // Run Ridders until we find the balance point.
         select_function<Iterator> func( start, finish, position, comm );
         return ridders( func, x1, x2 );
      }

   }
}

#endif
