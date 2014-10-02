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

#include "farmer.hh"

namespace hpc {
   namespace algorithm {

      farmer::farmer( int tag,
                      const mpi::comm& comm )
         : _max_its( 10 ),
           _tag( tag ),
           _comm( NULL )
      {
         set_comm( comm );
      }

      void
      farmer::set_comm( const mpi::comm& comm )
      {
         _comm = &comm;
      }

      range<unsigned long long>
      farmer::request()
      {
         // Send a request for more tasks.
         LOGDLN( "Sending request for tasks." );
         int flag = 1;
         _comm->send<int>( flag, 0, _tag );

         // Receive new task range.
         vector<unsigned long long> rng( 2 );
         _comm->recv<unsigned long long>( rng, 0, _tag );
         LOGDLN( "Received task range: [", rng[0], "-", rng[1], ")" );

         // If the range is empty it means we're done.
         return range<unsigned long long>( rng[0], rng[1] );
      }

   }
}
