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

#include <boost/range/algorithm/fill.hpp>
#include "surjection.hh"

namespace hpc {
   namespace mpi {

      surjection::surjection()
      {
      }

      mpi::scatter
      surjection::scatter( datatype const& type ) const
      {
      }

      hpc::view<std::vector<unsigned> >
      surjection::out_indices( int rank ) const
      {
	 return hpc::view<std::vector<unsigned> >( _out_idxs, _out_cnts[rank], _out_displs[rank] );
      }

      hpc::view<std::vector<unsigned> >
      surjection::inc_indices( int rank ) const
      {
	 return hpc::view<std::vector<unsigned> >( _local, _req_cnts[rank], _req_displs[rank] );
      }

      mpi::comm const&
      surjection::comm() const
      {
	 return *_comm;
      }

      std::vector<unsigned>
      surjection::_count_required( unsigned n_gelems,
				   std::vector<unsigned> const& idxs,
				   int n_ranks )
      {
	 std::vector<unsigned> cnts( n_ranks );
	 boost::fill( cnts, 0 );
	 for( unsigned ii = 0; ii < idxs.size(); ++ii )
	 {
	    int rank = balanced_locate_rank( n_gelems, n_ranks, idxs[ii] );
	    ++cnts[rank];
	 }
	 return cnts;
      }

      void
      surjection::_make_required( unsigned n_gelems,
				  std::vector<unsigned> const& idxs,
				  int n_ranks,
				  std::vector<unsigned>& req_idxs,
				  std::vector<unsigned>& req_cnts,
				  std::vector<unsigned> const& req_displs,
				  std::vector<unsigned>& local )
      {
	 boost::fill( req_cnts, 0 );
	 for( unsigned ii = 0; ii < idxs.size(); ++ii )
	 {
	    int rank = balanced_locate_rank( n_gelems, n_ranks, idxs[ii] );
	    req_idxs[req_displs[rank] + req_cnts[rank]] = idxs[ii];
	    local[req_displs[rank] + req_cnts[rank]] = ii;
	    req_cnts[rank]++;
	 }
      }

   }
}
