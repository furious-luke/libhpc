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

#include <mpi.h>
#include "libhpc/logging/logging.hh"
#include "libhpc/system/stream_output.hh"
#include "host.hh"

namespace hpc {
   namespace mpi {

      std::set<int>
      make_host_ranks( mpi::comm const& comm )
      {
         LOGBLOCKD( "Constructing host ranks." );

         // Get the hardware name.
         std::vector<char> name( MPI_MAX_PROCESSOR_NAME );
         int len;
         MPI_Get_processor_name( name.data(), &len );
         name.resize( len );
         LOGDLN( "Host name: ", std::string( name.begin(), name.end() ) );

         // Iterate over all ranks for broadcasting.
         std::set<int> ranks;
         for( unsigned ii = 0; ii < comm.size(); ++ii )
         {
            std::vector<char> rem_name = comm.bcast2<char>( name, ii );
            if( rem_name == name )
               ranks.insert( ii );
         }
         LOGDLN( "Ranks: ", ranks );

         return ranks;
      }

   }
}
