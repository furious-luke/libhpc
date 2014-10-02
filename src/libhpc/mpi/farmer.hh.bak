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

#ifndef hpc_algorithm_farmer_hh
#define hpc_algorithm_farmer_hh

#include "libhpc/debug/assert.hh"
#include "libhpc/logging/logging.hh"
#include "libhpc/mpi/comm.hh"

class farmer_suite;

namespace hpc {
   namespace algorithm {

      class farmer
      {
         friend class ::farmer_suite;

      public:

         farmer( int tag,
                 const mpi::comm& comm = mpi::comm::world );

         void
         set_comm( const mpi::comm& comm );

	 template< class Master >
         range<unsigned long long>
         serial_request( Master& me )
	 {
	    range<unsigned long long> rng( _first, _last );
	    me.next( _first, _last );
	    return rng;
	 }

         range<unsigned long long>
         request();

         template< class Master >
         void
         initialise( Master& me )
         {
            // Prepare the first batch.
            me.next( _first, _last );

            // Set remaining ranks.
            _rem = _comm->size() - 1;
         }

         template< class Master >
         bool
         master( Master& me )
         {
            LOGDLN( "Entering farming master." );

            // Wait for requests for more.
            MPI_Status stat;
            unsigned it = 0;
            while( it++ < _max_its && _comm->iprobe( stat, MPI_ANY_SOURCE, _tag ) )
            {
               LOGDLN( "Have an incoming request from rank: ", stat.MPI_SOURCE );

               int flag;
               _comm->recv<int>( flag, stat.MPI_SOURCE, _tag );

               // Do we have anythin remaining?
               vector<unsigned long long> rng( 2 );
               if( _first != _last )
               {
                  // Send out new batch.
                  rng[0] = _first;
                  rng[1] = _last;
                  LOGDLN( "Sending out range: [", rng[0], "-", rng[1], ")" );
                  _comm->send<unsigned long long>( rng, stat.MPI_SOURCE, _tag );
                  _first = _last;

                  // Update for next batch.
                  me.next( _first, _last );
               }
               else
               {
                  // Reply indicating empty range.
                  rng[0] = rng[1] = 0;
                  _comm->send<unsigned long long>( rng, stat.MPI_SOURCE, _tag );

                  // Decrement remaining ranks.
                  --_rem;
                  LOGDLN( "Signalling termination, ", _rem, " remaining ranks." );
               }
            }

            LOGDLN( "Leaving farming master, remaining ranks: ", _rem );
            return (_rem != 0);
         }

      protected:

         unsigned long long _first, _last;
         unsigned _rem;
         unsigned _max_its;
         int _tag;
         const mpi::comm* _comm;
      };

   }
}

#endif
