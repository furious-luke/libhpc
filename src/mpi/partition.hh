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

#ifndef hpc_mpi_partition_hh
#define hpc_mpi_partition_hh

#include <string>
#include <set>
#include <boost/range/numeric.hpp>
#include <boost/range/algorithm/fill.hpp>
#include <boost/range/algorithm_ext/iota.hpp>
#include "comm.hh"
#include "libhpc/algorithm/sort_by_key.hh"
#include "libhpc/system/type_traits.hh"

namespace hpc {
   namespace mpi {

      class balanced_partition
      {
      public:

	 balanced_partition( mpi::comm const& comm = mpi::comm::world );

	 void
	 clear();

	 template< class Iter >
	 mpi::comm
	 construct( Iter const& begin,
		    Iter const& end )
	 {
	    clear();
	    if( _comm->size() == 1 )
	       return _construct_serial( begin, end );
	    else
	       return _construct_parallel( begin, end );
	 }

	 void
	 transfer( void* data,
		   mpi::datatype const& type );

	 template< class Vec >
	 void
	 transfer( typename type_traits<Vec>::reference data )
	 {
	    typedef typename type_traits<Vec>::value::value_type value_type;
	    transfer( data.data(), mpi::datatype( MPI_MAP_TYPE( value_type ) ) );
	 }

	 template< class T >
	 void
	 transfer( T&& data )
	 {
	    transfer<T>( std::forward<T>( data ) );
	 }

	 mpi::comm const&
	 comm() const;

	 std::vector<unsigned> const
	 displs() const;

	 std::vector<unsigned> const
	 indices() const;

	 std::vector<int> const
	 partners() const;

      protected:

	 template< class Iter >
	 mpi::comm
	 _construct_serial( Iter const& begin,
			    Iter const& end )
	 {
	    LOGBLOCKD( "Constructing serial balanced partition." );

	    // Sanity checks.
	    ASSERT( std::accumulate( begin, end, 0 ) == balanced_right_size( end - begin, *_comm ),
		    "Partition is not balanced." );

	    // In serial we just need to collect the indices that will
	    // belong on the left.
	    _idxs.reserve( balanced_left_size( end - begin, *_comm ) );
	    for( Iter it = begin; it != end; ++it )
	    {
	       if( *it == 0 )
		  _idxs.push_back( it - begin );
	    }

	    return mpi::comm::self;
	 }

	 template< class Iter >
	 mpi::comm
	 _construct_parallel( Iter const& begin,
			      Iter const& end )
	 {
	    LOGBLOCKD( "Constructing parallel balanced partition." );

	    mpi::comm const& comm = *_comm;
	    int my_rank = comm.rank();

	    // Be sure we have the correct distribution.
	    ASSERT( end - begin == balanced_local_size( end - begin, *_comm ),
		    "Distributed array must be balanced prior to partition." );
	    ASSERT( comm.all_reduce( std::accumulate( begin, end, 0 ) ) == balanced_right_size( end - begin, *_comm ),
		    "Partition does not respect processor boundaries." );

	    // Sum the number of elements on the left.
	    long on_left = 0;
	    for( Iter it = begin; it != end; ++it )
	       on_left += ((*it) ? 0 : 1);
	    LOGDLN( on_left, " elements initially on left." );

	    // Gather them all to determine which ranks will be collecting for
	    // which side.
	    std::vector<long> balances = comm.all_gather( on_left );
	    std::vector<int> ranks( balances.size() );
            boost::iota( ranks, 0 );
            sort_by_key( balances, ranks );
	    LOGDLN( "On left vector: ", balances );
	    LOGDLN( "Rank vector:    ", ranks );

	    // Assign processors to left or right.
	    bool collecting_left = false;
	    for( unsigned ii = balances.size(); ii > balances.size() - (_comm->size()/2 + _comm->size()%2); --ii )
	    {
	       if( ranks[ii - 1] == my_rank )
	       {
		  collecting_left = true;
		  break;
	       }
	    }

	    // Reformulate the on_left value to indicate how many elements I need.
	    // Positive the value is how many lefts we need. If it's negative,
	    // it's how many lefts we need.
	    if( collecting_left )
	    {
	       on_left = (end - begin) - on_left;
	       LOGDLN( "Collecting for left." );
	    }
	    else
	    {
	       on_left *= -1;
	       LOGDLN( "Collecting for right." );
	    }

	    // Perform a second gather all.
	    comm.all_gather( on_left, balances );
            boost::iota( ranks, 0 );
            sort_by_key( balances, ranks );
	    std::reverse( balances.begin(), balances.end() );
	    std::reverse( ranks.begin(), ranks.end() );
	    LOGDLN( "Balance vector: ", balances );
	    LOGDLN( "Rank vector:    ", ranks );

	    unsigned n_exs, n_idxs;
	    for( int phase = 0; phase < 3; ++phase )
	    {
	       LOGBLOCKD( "Phase: ", phase );
	       std::vector<long> bals = balances;

	       if( phase == 0 )
		  n_idxs = 0;
	       else if( phase == 1 )
	       {
		  _displs.resize( n_exs + 1 );
		  boost::fill( _displs, 0 );
		  _idxs.resize( n_idxs );
		  _partners.resize( n_exs );
	       }
	       else if( phase == 2 )
	       {
		  LOGDLN( "Counts: ", hpc::view<std::vector<unsigned> >( _displs, _displs.size() - 1 ) );
		  counts_to_displs_i( _displs );
		  LOGDLN( "Displs: ", _displs );
	       }

	       unsigned left = 0, right = ranks.size() - 1;
	       Iter it = begin;
	       n_exs = 0;
	       while( left < ranks.size() && bals[left] > 0 )
	       {
		  LOGDLN( "Left: ", left );
		  LOGDLN( "Right: ", right );
		  LOGDLN( "Current left balance: ", bals[left] );
		  LOGDLN( "Current right balance: ", bals[right] );
		  ASSERT( bals[right] < 0, "Internal partition error: cannot have more on left than right." );

		  unsigned to_ex = std::min( bals[left], -bals[right] );
		  LOGDLN( "To exchange: ", to_ex );
		  if( ranks[left] == my_rank || ranks[right] == my_rank )
		  {
		     if( phase == 0 )
		     {
			++n_exs;
			n_idxs += to_ex;
		     }
		     else
		     {
			if( phase == 2 )
			{
			   int partner = (ranks[left] == my_rank) ? ranks[right] : ranks[left];
			   _partners[n_exs] = partner;
			}
			for( unsigned ii = 0; ii < to_ex; ++it )
			{
			   if( *it == (collecting_left ? 1 : 0) )
			   {
			      LOGDLN( "Going to transfer at position: ", it - begin );
			      if( phase == 2 )
			      {
				 LOGDLN( "Inserting into array at position: ", _displs[n_exs] );
				 _idxs[_displs[n_exs]] = it - begin;
			      }
			      ++_displs[n_exs];
			      ++ii;
			   }
			}
			++n_exs;
		     }
		  }
		  bals[left] -= to_ex;
		  bals[right] += to_ex;
		  if( !bals[left] )
		     ++left;
		  if( !bals[right] )
		     --right;
	       }

	       // Balances must have been cleared.
	       ASSERT( boost::accumulate( bals, 0 ) == 0, "Failed to balance partition." );
	    }

	    // Correct the displacements.
	    if( _displs.size() )
	    {
	       unsigned tmp = _displs[0];
	       _displs[0] = 0;
	       for( unsigned ii = 1; ii < _displs.size(); ++ii )
	       {
		  unsigned tmp2 = _displs[ii];
		  _displs[ii] = tmp;
		  tmp = tmp2;
	       }
	    }

	    return _comm->split( collecting_left ? 0 : 1 );
	 }

      protected:

	 std::vector<unsigned> _displs;
	 std::vector<unsigned> _idxs;
	 std::vector<int> _partners;
	 mpi::comm const* _comm;
      };

   }
}

#endif
