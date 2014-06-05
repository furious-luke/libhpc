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

#include <boost/function.hpp>
#include "libhpc/debug/assert.hh"
#include "libhpc/logging.hh"
#include "libhpc/mpi/comm.hh"

namespace hpc {
   namespace mpi {

      template< class IndexT >
      class indexer
      {
      public:

         typedef IndexT index_type;

      public:

         indexer( int tag,
                  mpi::comm const& comm = mpi::comm::world )
            : _base{ 0 },
              _max_its( 10 ),
              _tag( tag ),
              _comm( NULL )
         {
            set_comm( comm );
         }

         void
         set_comm( mpi::comm const& comm )
         {
            _comm = &comm;
         }

         void
         set_max_its( unsigned its )
         {
            _max_its = its;
         }

         index_type
         request( index_type size,
                  boost::function<void(index_type,index_type)> cb = boost::function<void(index_type,index_type)>() )
         {
            index_type base;
            if( _comm->size() > 1 )
            {
               LOGDLN( "Requesting ", size, " indices." );
               _comm->send( size, 0, _tag );
               _comm->recv( base, 0, _tag );
               LOGDLN( "Received index base: ", base );
               if( cb )
                  cb( base, size );
            }
            else
            {
               base = _base;
               if( cb )
                  cb( _base, size );
               _base += size;
            }
            return base;
         }

	 void
	 master( boost::function<void(index_type,index_type)> cb = boost::function<void(index_type,index_type)>() )
         {
            // Must be the master to run this.
            ASSERT( _comm->rank() == 0 );

            // If we are running serially then skip this part.
            if( _comm->size() > 1 )
            {
               MPI_Status stat;
               unsigned it = 0;
               while( it++ < _max_its && _comm->iprobe( stat, MPI_ANY_SOURCE, _tag ) )
               {
                  LOGDLN( "Have an incoming index request from: ", stat.MPI_SOURCE );
                  unsigned long long size;
                  _comm->recv( size, stat.MPI_SOURCE, _tag );
                  LOGDLN( "Requested ", size, " indices." );
                  if( cb )
                     cb( _base, size );
                  _comm->send( _base, stat.MPI_SOURCE, _tag );
                  _base += size;
               }
            }
         }

         index_type
	 base() const;

      protected:

         
         index_type _base;
         unsigned _max_its;
         int _tag;
         mpi::comm const* _comm;
      };

   }
}

#endif
