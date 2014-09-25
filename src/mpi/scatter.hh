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

#ifndef hpc_mpi_scatter_hh
#define hpc_mpi_scatter_hh

#include "comm.hh"
#include "datatype.hh"

namespace hpc {
   namespace mpi {

      class surjection;

      class scatter
      {
      public:

	 scatter();

	 scatter( surjection const& srj,
		  datatype const& type );

	 template< class IdxsT >
	 scatter( unsigned n_gelems,
		  IdxsT const& idxs,
		  datatype const& type,
		  mpi::comm const& comm = mpi::comm::world )
	 {
	    construct( n_gelems, idxs, type, comm );
	 }

	 scatter( scatter&& src );

	 void
	 clear();

	 void
	 construct( surjection const& srj,
		    datatype const& type );

	 template< class IdxsT >
	 void
	 construct( unsigned n_gelems,
		    IdxsT const& idxs,
		    datatype const& type,
		    mpi::comm const& comm = mpi::comm::world )
	 {
	    surjection srj( n_gelems, idxs, comm );
	    construct( srj, type );
	 }

	 template< class SrcT,
		   class DstT >
	 inline
	 void
	 transfer( SrcT const& src,
		   DstT&& dst )
	 {
	    _transfer<SrcT,DstT>( src, std::forward<DstT>( dst ) );
	 }

      protected:

	 template< class SrcT,
		   class DstT >
	 void
	 _transfer( SrcT const& src,
		    typename type_traits<DstT>::reference dst )
	 {
	    _comm->alltoallw( src.data(), _ones.data(), _zeros.data(), _out_types.data(),
			      dst.data(), _ones.data(), _zeros.data(), _inc_types.data() );
	 }

      protected:

	 std::vector<mpi::datatype> _inc_types, _out_types;
	 std::vector<int> _zeros, _ones;
	 mpi::comm const* _comm;
      };

   }
}

#endif
