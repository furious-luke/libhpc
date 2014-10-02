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

#include <map>
#include "libhpc/debug/assert.hh"
#include "libhpc/system/assign.hh"
#include "libhpc/algorithm/sequence_checks.hh"
#include "libhpc/algorithm/counts.hh"
#include "comm.hh"
#include "requests.hh"

namespace hpc {
   namespace mpi {

      class vct
      {
      public:

	 vct( mpi::comm const& comm = mpi::comm::null );

#ifdef CXX_0X

         template< class Seq >
	 vct( Seq&& nbrs,
	      mpi::comm const& comm = mpi::comm::world )
            : _comm( (mpi::comm const*)&comm )
         {
            set_neighbors( std::forward<Seq>( nbrs ) );
         }

#else

         template< class Seq >
	 vct( Seq& nbrs,
	      mpi::comm const& comm = mpi::comm::world )
            : _comm( (mpi::comm const*)&comm )
         {
            set_neighbors( nbrs );
         }

         template< class Seq >
	 vct( Seq const& nbrs,
	      mpi::comm const& comm = mpi::comm::world )
            : _comm( (mpi::comm const*)&comm )
         {
            set_neighbors( nbrs );
         }

#endif

	 ~vct();

	 void
	 clear();

	 void
	 set_comm( mpi::comm const& comm );

	 mpi::comm const&
	 comm() const;

#ifdef CXX_0X

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

#else

         template< class Seq >
	 void
	 set_neighbors( Seq& nbrs )
         {
            ASSERT( !has_duplicates( nbrs ), "Duplicates in neighbors." );
            ASSERT( !has_element( nbrs, _comm->rank() ), "Neighbors contains my rank." );
            ASSERT( is_ordered( nbrs ), "Neighbors are not ordered." );
            clear();
            hpc::assign( _nbrs, nbrs );
            for( unsigned ii = 0; ii < _nbrs.size(); ++ii )
               _rtn.insert( std::make_pair( _nbrs[ii], ii ) );
         }

         template< class Seq >
	 void
	 set_neighbors( Seq const& nbrs )
         {
            ASSERT( !has_duplicates( nbrs ), "Duplicates in neighbors." );
            ASSERT( !has_element( nbrs, _comm->rank() ), "Neighbors contains my rank." );
            ASSERT( is_ordered( nbrs ), "Neighbors are not ordered." );
            clear();
            hpc::assign( _nbrs, nbrs );
            for( unsigned ii = 0; ii < _nbrs.size(); ++ii )
               _rtn.insert( std::make_pair( _nbrs[ii], ii ) );
         }

#endif

         unsigned
	 n_neighbors() const;

         std::vector<unsigned> const&
	 neighbors() const;

         unsigned
         rank_to_nbr( unsigned rank ) const;

	 template< class T >
	 void
	 ibcast( T const& out,
		 view<std::vector<T> > inc,
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
                view<std::vector<T> > inc,
                int tag = 0 ) const
         {
            mpi::requests reqs;
            ibcast( out, inc, reqs, tag );
         }

	 void
	 iscatter( void const* out,
                   void* inc,
                   mpi::datatype const& type,
                   mpi::requests& reqs,
                   unsigned block_size = 1,
                   int tag = 0 ) const;

	 template< class T >
	 void
	 iscatter( view<std::vector<T> > const& out,
		   view<std::vector<T> > inc,
		   mpi::requests& reqs,
		   int tag = 0 ) const
         {
            ASSERT( out.size() == _nbrs.size() );
            ASSERT( inc.size() == _nbrs.size() );
	    mpi::datatype type( MPI_MAP_TYPE( T ) );
            iscatter( out.data(), inc.data(), type, reqs, 1, tag );
         }

	 template< class T >
	 void
	 scatter( view<std::vector<T> > const& out,
                  view<std::vector<T> > inc,
                  int tag = 0 ) const
         {
            mpi::requests reqs;
            iscatter<T>( out, inc, reqs, tag );
         }

         template< class Index >
	 void
	 iscatter( void* out,
                   view<std::vector<Index> > const& out_displs,
                   void* inc,
                   view<std::vector<Index> > inc_displs,
                   mpi::datatype const& type,
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
               bcast<unsigned>( block_size, check_blocks );
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
			     out_displs[ii + 1] - out_displs[ii],
                             tag );
            }
            for( unsigned ii = 0; ii < num_nbrs; ++ii )
            {
               _comm->irecv( (uint8_t*)inc + inc_displs[ii]*raw_block_size,
                             type,
                             _nbrs[ii],
                             reqs[cur_req++],
			     inc_displs[ii + 1] - inc_displs[ii],
                             tag );
            }
         }

         template< class T,
                   class Index >
	 void
	 iscatter( view<std::vector<T> > const& out,
                   view<std::vector<Index> > const& out_displs,
                   view<std::vector<T> > inc,
                   view<std::vector<Index> > inc_displs,
                   mpi::requests& reqs,
                   int tag = 0 ) const
         {
	    mpi::datatype type( MPI_MAP_TYPE( T ) );
            iscatter<Index>( out.data(), out_displs, inc.data(), inc_displs, type, reqs, 1, tag );
         }

         template< class T,
                   class Index >
	 void
	 scatter( view<std::vector<T> > const& out,
                  view<std::vector<Index> > const& out_displs,
                  view<std::vector<T> > inc,
                  view<std::vector<Index> > inc_displs,
                  int tag = 0 ) const
         {
            mpi::requests reqs;
            iscatter<T,Index>( out, out_displs, inc, inc_displs, reqs, tag );
         }

         template< class T,
                   class Index >
         void
         scattera( view<std::vector<T> > const& out,
                   view<std::vector<Index> > const& out_displs,
                   std::vector<T>& inc,
                   std::vector<Index>& inc_displs,
                   int tag = 0 ) const
         {
	    if( _nbrs.size() )
	    {
	       inc_displs.resize( _nbrs.size() + 1 );
	       scatter_displs<Index>( out_displs, inc_displs );
	       inc.resize( inc_displs.back() );
	       scatter<T,Index>( out, out_displs, inc, inc_displs, tag );
	    }
	    else
	    {
	       hpc::deallocate( inc );
	       hpc::deallocate( inc_displs );
	    }
         }

	 template< class Iter >
	 void
	 iscatter_types( void const* out,
			 Iter out_datatypes,
			 void* inc,
			 Iter inc_datatypes,
			 mpi::requests& reqs ) const
	 {
            unsigned num_nbrs = _nbrs.size();
            reqs.resize( 2*num_nbrs );
	    unsigned cur_req = 0;
	    for( unsigned ii = 0; ii < num_nbrs; ++ii )
	    {
	       _comm->isend( out, *out_datatypes++, _nbrs[ii], reqs[cur_req++] );
	       _comm->irecv( inc, *inc_datatypes++, _nbrs[ii], reqs[cur_req++] );
	    }
	 }

	 template< class Iter >
	 void
	 scatter_types( void const* out,
			Iter out_datatypes,
			void* inc,
			Iter inc_datatypes ) const
	 {
	    mpi::requests reqs;
	    iscatter_types<Iter>( out, out_datatypes, inc, inc_datatypes, reqs );
	 }

	 template< class DisplType >
	 void
	 scatter_displs( view<std::vector<DisplType> > const& out,
			 view<std::vector<DisplType> > inc,
			 int tag = 0 ) const
	 {
            typedef view<std::vector<DisplType> > view_type;

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
         std::map<unsigned,unsigned> _rtn;
	 mpi::comm const* _comm;
      };
   }
}

#endif
