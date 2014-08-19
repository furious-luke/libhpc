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

#ifndef hpc_mpi_indexer_hh
#define hpc_mpi_indexer_hh

#include "libhpc/system/cc_version.hh"
#include "libhpc/debug/assert.hh"
#include "libhpc/logging.hh"
#include "libhpc/mpi/comm.hh"
#include "async.hh"

namespace hpc {
   namespace mpi {

      template< class IndexT >
      class indexer
         : public async::event_handler
      {
      public:

         typedef IndexT index_type;

      public:

         indexer( int tag,
                  mpi::comm const& comm = mpi::comm::world )
            : async::event_handler( tag ),
#ifdef CXX_0X
              _base{ 0 },
#endif
              _comm( &comm )
         {
#ifndef CXX_0X
            _base = 0;
#endif
         }

         void
         set_comm( mpi::comm const& comm )
         {
            _comm = &comm;
         }

         mpi::comm const&
         comm() const
         {
            return *_comm;
         }

         index_type
         request( index_type const& size )
         {
            LOGBLOCKD( "Indexer requesting indices: ", size );
            index_type base;
            if( _comm->size() > 1 )
            {
               _comm->send( size, 0, tag() );
               _comm->recv( base, 0, tag() );
            }
            else
            {
               base = _base;
               _base += size;
            }
            LOGDLN( "Received index base: ", base );
            return base;
         }

         bool
	 event( MPI_Status const& stat )
         {
            ASSERT( _comm->rank() == 0, "Must be master to handle indexer events." );
            ASSERT( _comm->size() > 1, "Must be running in parallel to handle indexer events." );

            LOGBLOCKD( "Have an incoming index request from: ", stat.MPI_SOURCE );
            index_type size;
            _comm->recv( size, stat.MPI_SOURCE, tag() );
            LOGDLN( "Requested indices: ", size );
            _comm->send( _base, stat.MPI_SOURCE, tag() );
            _base += size;

            return false;
         }

         index_type const&
	 base() const
         {
            return _base;
         }

      protected:

         
         index_type _base;
         mpi::comm const* _comm;
      };

   }
}

#endif
