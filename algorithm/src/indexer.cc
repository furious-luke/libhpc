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

#include "libhpc/logging/logging.hh"
#include "indexer.hh"

namespace hpc {
   namespace algorithm {

      indexer::indexer( int tag,
                        const mpi::comm& comm )
         : _base( 0 ),
           _max_its( 10 ),
           _tag( tag ),
           _comm( NULL )
      {
         set_comm( comm );
      }

      void
      indexer::set_comm( const mpi::comm& comm )
      {
         _comm = &comm;
      }

      void
      indexer::set_max_its( unsigned its )
      {
         _max_its = its;
      }

      unsigned long long
      indexer::request( unsigned long long size )
      {
         unsigned long long base;
         LOGDLN( "Requesting ", size, " indices." );
         _comm->send( size, 0, _tag );
         _comm->recv( base, 0, _tag );
         LOGDLN( "Received index base: ", base );
         return base;
      }

      void
      indexer::master()
      {
         // Must be the master to run this.
         ASSERT( _comm->rank() == 0 );

         MPI_Status stat;
         unsigned it = 0;
         while( it++ < _max_its && _comm->iprobe( stat, MPI_ANY_SOURCE, _tag ) )
         {
            LOGDLN( "Have an incoming index request from: ", stat.MPI_SOURCE );
            unsigned long long size;
            _comm->recv( size, stat.MPI_SOURCE, _tag );
            LOGDLN( "Requested ", size, " indices." );
            _comm->send( _base, stat.MPI_SOURCE, _tag );
            _base += size;
         }
      }

   }
}
