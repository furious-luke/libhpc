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

#include "vct.hh"

namespace hpc {
   namespace mpi {

      vct::vct( mpi::comm const& comm )
	 : _comm( (mpi::comm*)&comm )
      {
      }

      vct::~vct()
      {
      }

      void
      vct::clear()
      {
         hpc::deallocate( _nbrs );
         hpc::deallocate( _rtn );
      }

      void
      vct::set_comm( mpi::comm const& comm )
      {
	 clear();
	 _comm.reset( (mpi::comm*)&comm );
      }

      mpi::comm const&
      vct::comm() const
      {
	 return *_comm.get();
      }

      unsigned
      vct::n_neighbors() const
      {
	 return _nbrs.size();
      }

      std::vector<unsigned> const&
      vct::neighbors() const
      {
         return _nbrs;
      }

      unsigned
      vct::rank_to_nbr( unsigned rank ) const
      {
         return _rtn.at( rank );
      }

      void
      vct::iscatter( void const* out,
		     void* inc,
		     mpi::data_type const& type,
		     mpi::requests& reqs,
                     unsigned block_size,
		     int tag ) const
      {
	 unsigned num_nbrs = this->_nbrs.size();

#ifndef NDEBUG
	 // Check that every rank involved is sending the same number of blocks.
	 {
            std::vector<unsigned> check_cnts( num_nbrs );
	    bcast( block_size, check_cnts );
	    for( unsigned ii = 0; ii < check_cnts.size(); ++ii )
	       ASSERT( check_cnts[ii] == block_size, "VCT iscatter block sizes don't match." );
	 }
#endif

	 reqs.resize( 2*num_nbrs );
	 unsigned raw_block_size = type.size()*block_size;
	 unsigned cur_req = 0;
	 for( unsigned ii = 0; ii < num_nbrs; ++ii )
	    _comm->isend( (uint8_t*)out + ii*raw_block_size, type, _nbrs[ii], reqs[cur_req++], block_size, tag );
	 for( unsigned ii = 0; ii < num_nbrs; ++ii )
	    _comm->irecv( (uint8_t*)inc + ii*raw_block_size, type, _nbrs[ii], reqs[cur_req++], block_size, tag );
      }

   }
}
