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

#include "partition.hh"

namespace hpc {
   namespace mpi {

      balanced_partition::balanced_partition( mpi::comm const& comm )
	 : _comm( &comm )
      {
      }

      void
      balanced_partition::clear()
      {
	 hpc::deallocate( _displs );
	 hpc::deallocate( _idxs );
	 hpc::deallocate( _partners );
      }

      void
      balanced_partition::transfer( void* data,
				    mpi::datatype const& type )
      {
	 if( _partners.size() > 0 )
	 {
	    unsigned type_size = type.size();
	    for( unsigned ii = 0; ii < _partners.size(); ++ii )
	    {
	       unsigned n_idxs = _displs[ii + 1] - _displs[ii];
	       hpc::view<std::vector<unsigned> > idxs( _idxs, n_idxs, _displs[ii] );
	       mpi::datatype idx_type = type.indexed( idxs );
	       std::vector<char> buf( type_size*n_idxs );
	       _comm->exchange( data, 1, idx_type, buf.data(), n_idxs, type, _partners[ii] );
	       for( unsigned jj = 0; jj < n_idxs; ++jj )
		  memcpy( (char*)data + type_size*idxs[jj], buf.data() + type_size*jj, type_size );
	    }
	 }
	 else
	 {
	    unsigned type_size = type.size();
	    std::vector<char> buf( type_size );
	    unsigned dst = 0, src = 0;
	    for( ; src != _idxs.size(); ++src, ++dst )
	    {
	       if( dst != _idxs[src] )
	       {
		  memcpy( buf.data(), (char*)data + dst*type_size, type_size );
		  memcpy( (char*)data + dst*type_size, (char*)data + _idxs[src]*type_size, type_size );
		  memcpy( (char*)data + _idxs[src]*type_size, buf.data(), type_size );
	       }
	    }
	 }
      }

      mpi::comm const&
      balanced_partition::comm() const
      {
	 return *_comm;
      }

      std::vector<unsigned> const
      balanced_partition::displs() const
      {
	 return _displs;
      }

      std::vector<unsigned> const
      balanced_partition::indices() const
      {
	 return _idxs;
      }

      std::vector<int> const
      balanced_partition::partners() const
      {
	 return _partners;
      }

   }
}
