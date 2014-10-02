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

#ifndef hpc_mpi_surjection_hh
#define hpc_mpi_surjection_hh

#include "comm.hh"
#include "datatype.hh"
#include "helpers.hh"
#include "scatter.hh"

namespace hpc {
   namespace mpi {

      class surjection
      {
      public:

	 surjection();

	 template< class IdxsT >
	 surjection( unsigned n_gelems,
		     IdxsT const& idxs,
		     mpi::comm const& comm = mpi::comm::world )
	 {
	    construct( n_gelems, idxs, comm );
	 }

	 template< class IdxsT >
	 void
	 construct( unsigned n_gelems,
		    IdxsT const& idxs,
		    mpi::comm const& comm = mpi::comm::world )
	 {
	    int n_ranks = comm.size();

	    // Count the number of required elements coming from
	    // each processor, using a full array.
	    _req_cnts = _count_required( n_gelems, idxs, n_ranks );
	    _req_displs = counts_to_displs( _req_cnts );

	    // Calculate required indices.
	    std::vector<unsigned> req_idxs( _req_displs[n_ranks] );
	    _local.resize( _req_displs[n_ranks] );
	    _make_required( n_gelems, idxs, n_ranks, req_idxs, _req_cnts, _req_displs, _local );

	    // Send information about sizes.
	    _out_cnts.resize( n_ranks );
	    comm.alltoall( _req_cnts, _out_cnts );
	    _out_displs = counts_to_displs( _out_cnts );

	    // Send information about required indices.
	    _out_idxs.resize( _out_displs[n_ranks] );
	    comm.alltoallv( req_idxs, _req_cnts, _req_displs, _out_idxs, _out_cnts, _out_displs );
	    hpc::deallocate( req_idxs );

	    // Use a scan to find my base.
	    unsigned n_lelems = balanced_local_sizeg( n_gelems, comm );
	    unsigned base = comm.scan( n_lelems );

	    // Ensure outgoing indices are local indices.
	    for( unsigned ii = 0; ii < _out_displs[n_ranks]; ++ii )
	    {
	       ASSERT( _out_idxs[ii] >= base && _out_idxs[ii] < base + n_lelems );
	       _out_idxs[ii] -= base;
	    }

	    _comm = &comm;
	 }

	 mpi::scatter
	 scatter( datatype const& type ) const;

	 hpc::view<std::vector<unsigned> >
	 out_indices( int rank ) const;

	 hpc::view<std::vector<unsigned> >
	 inc_indices( int rank ) const;

	 mpi::comm const&
	 comm() const;

      protected:

	 std::vector<unsigned>
	 _count_required( unsigned n_gelems,
			  std::vector<unsigned> const& idxs,
			  int n_ranks );

	 void
	 _make_required( unsigned n_gelems,
			 std::vector<unsigned> const& idxs,
			 int n_ranks,
			 std::vector<unsigned>& req_idxs,
			 std::vector<unsigned>& req_cnts,
			 std::vector<unsigned> const& req_displs,
			 std::vector<unsigned>& local );

      protected:

	 std::vector<unsigned> _req_cnts, _req_displs;
	 std::vector<unsigned> _out_cnts, _out_displs, _out_idxs;
	 std::vector<unsigned> _local;
	 mpi::comm const* _comm;
      };

   }
}

#endif
