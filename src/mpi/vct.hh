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

#ifndef libhpc_mpi_vct_hh
#define libhpc_mpi_vct_hh

#include <unordered_map>
#include "libhpc/debug/assert.hh"
#include "libhpc/containers/helpers.hh"
#include "libhpc/containers/counts.hh"
#include "comm.hh"
#include "requests.hh"

namespace hpc {
   namespace mpi {

      class vct
      {
      public:

	 vct( mpi::comm const& comm = mpi::comm::null );

         template< class Seq >
	 vct( Seq&& nbrs,
	      mpi::comm const& comm = mpi::comm::world )
            : _comm( (mpi::comm*)&comm )
         {
            set_neighbors( std::forward<Seq>( nbrs ) );
         }

	 ~vct();

	 void
	 clear();

	 void
	 set_comm( mpi::comm const& comm );

	 mpi::comm const&
	 comm() const;

         template< class Seq >
	 void
	 set_neighbors( Seq&& nbrs )
         {
            ASSERT( !has_duplicates( nbrs ), "Duplicates in neighbors." );
            ASSERT( !has_element( nbrs, _comm->rank() ), "Neighbors contains my rank." );
            ASSERT( is_ordered( nbrs ), "Neighbors are not ordered." );
            clear();
            hpc::assign( _nbrs, std::forward<Seq>( nbrs ) );
            for( unsigned ii = 0; ii < _nbrs.size(); ++ii )
               _rtn.emplace( _nbrs[ii], ii );
         }

         unsigned
	 n_neighbors() const;

         std::vector<unsigned> const&
	 neighbors() const;

         unsigned
         rank_to_nbr( unsigned rank ) const;

	 template< class T >
	 void
	 ibcast( T const& out,
		 typename hpc::view<std::vector<T>>::type inc,
                 mpi::requests& reqs,
		 int tag = 0 ) const
         {
            int num_nbrs = _nbrs.size();
            reqs.resize( 2*num_nbrs );
            unsigned cur_req = 0;
            for( unsigned ii = 0; ii < num_nbrs; ++ii )
               _comm->isend( out, _nbrs[ii], reqs[cur_req++], tag );
            for( unsigned ii = 0; ii < num_nbrs; ++ii )
               _comm->irecv( inc[ii], _nbrs[ii], reqs[cur_req++], tag);
         }

         template< class T >
         void
         bcast( T const& out,
                typename hpc::view<std::vector<T>>::type inc,
                int tag = 0 ) const
         {
            mpi::requests reqs;
            ibcast( out, inc, reqs, tag );
         }

	 void
	 iscatter( void const* out,
                   void* inc,
                   mpi::data_type const& type,
                   mpi::requests& reqs,
                   unsigned block_size = 1,
                   int tag = 0 ) const;

	 template< class T >
	 void
	 iscatter( typename hpc::view<std::vector<T>>::type const& out,
		   typename hpc::view<std::vector<T>>::type inc,
		   mpi::requests& reqs,
		   int tag = 0 ) const
         {
            ASSERT( out.size() == _nbrs.size() );
            ASSERT( inc.size() == _nbrs.size() );
	    mpi::data_type type( MPI_MAP_TYPE( T ) );
            iscatter( out.data(), inc.data(), type, reqs, 1, tag );
         }

	 template< class T >
	 void
	 scatter( typename hpc::view<std::vector<T>>::type const& out,
                  typename hpc::view<std::vector<T>>::type inc,
                  int tag = 0 ) const
         {
            mpi::requests reqs;
            iscatter<T>( out, inc, reqs, tag );
         }

         template< class Index >
	 void
	 iscatter( void* out,
                   typename hpc::view<std::vector<Index>>::type const& out_displs,
                   void* inc,
                   typename hpc::view<std::vector<Index>>::type inc_displs,
                   mpi::data_type const& type,
                   mpi::requests& reqs,
                   unsigned block_size = 1,
                   int tag = 0 ) const
         {
#ifndef NDEBUG
            // Check that the outgoing counts match the incoming counts.
            {
               std::vector<Index> check_cnts( _nbrs.size() ), out_cnts;
               hpc::displs_to_counts_resize( out_displs, out_cnts );
               scatter<Index>( out_cnts, check_cnts );
               for( unsigned ii = 0; ii < check_cnts.size(); ++ii )
                  ASSERT( check_cnts[ii] == (inc_displs[ii + 1] - inc_displs[ii]), "VCT scatter sizes don't match." );
            }
            // Check that the number of blocks involved is constant.
            {
               std::vector<unsigned> check_blocks( _nbrs.size() );
               bcast( block_size, check_blocks );
               for( unsigned ii = 0; ii < check_blocks.size(); ++ii )
                  ASSERT( check_blocks[ii] == block_size, "VCT scatter block sizes don't match." );
            }
#endif

            unsigned num_nbrs = _nbrs.size();
            reqs.resize( 2*num_nbrs );
            unsigned raw_block_size = type.size()*block_size;
            unsigned cur_req = 0;
            for( unsigned ii = 0; ii < num_nbrs; ++ii )
            {
               _comm->isend( (uint8_t*)out + out_displs[ii]*raw_block_size,
                             type,
                             _nbrs[ii],
                             reqs[cur_req++],
                             block_size,
                             tag );
            }
            for( unsigned ii = 0; ii < num_nbrs; ++ii )
            {
               _comm->irecv( (uint8_t*)inc + inc_displs[ii]*raw_block_size,
                             type,
                             _nbrs[ii],
                             reqs[cur_req++],
                             block_size,
                             tag );
            }
         }

         template< class T,
                   class Index >
	 void
	 iscatter( typename hpc::view<std::vector<T>>::type const& out,
                   typename hpc::view<std::vector<Index>>::type const& out_displs,
                   typename hpc::view<std::vector<T>>::type inc,
                   typename hpc::view<std::vector<Index>>::type inc_displs,
                   mpi::requests& reqs,
                   int tag = 0 ) const
         {
	    mpi::data_type type( MPI_MAP_TYPE( T ) );
            iscatter<Index>( out.data(), out_displs, inc.data(), inc_displs, type, reqs, 1, tag );
         }

         template< class T,
                   class Index >
	 void
	 scatter( typename hpc::view<std::vector<T>>::type const& out,
                  typename hpc::view<std::vector<Index>>::type const& out_displs,
                  typename hpc::view<std::vector<T>>::type inc,
                  typename hpc::view<std::vector<Index>>::type inc_displs,
                  int tag = 0 ) const
         {
            mpi::requests reqs;
            iscatter<T,Index>( out, out_displs, inc, inc_displs, reqs, tag );
         }

         template< class T,
                   class Index >
         void
         scattera( typename hpc::view<std::vector<T>>::type const& out,
                   typename hpc::view<std::vector<Index>>::type const& out_displs,
                   std::vector<T>& inc,
                   std::vector<Index>& inc_displs,
                   int tag = 0 ) const
         {
            inc_displs.resize( _nbrs.size() + 1 );
            scatter_displs<Index>( out_displs, inc_displs );
            inc.resize( inc_displs.back() );
            scatter<T,Index>( out, out_displs, inc, inc_displs, tag );
         }

	 template< class DisplType >
	 void
	 scatter_displs( typename hpc::view<std::vector<DisplType>>::type const& out,
			 typename hpc::view<std::vector<DisplType>>::type inc,
			 int tag = 0 ) const
	 {
            typedef typename hpc::view<std::vector<DisplType>>::type view_type;

	    ASSERT( out.size() == inc.size() );
	    if( out.size() > 1 )
            {
	       hpc::displs_to_counts<view_type>( (view_type&)out );
	       scatter<DisplType>( view_type( out, out.size() - 1 ), view_type( inc, inc.size() - 1 ) );
	       hpc::counts_to_displs<view_type>( (view_type&)out );
	       hpc::counts_to_displs<view_type>( inc );
	    }
	 }

      protected:

         std::vector<unsigned> _nbrs;
         std::unordered_map<unsigned,unsigned> _rtn;
	 std::shared_ptr<mpi::comm> _comm;
      };
   }
}

#endif
