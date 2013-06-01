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

#include "indexer.hh"

namespace hpc {
   namespace algorithm {

      indexer::indexer( int tag,
                        const mpi::comm& comm )
         : _base( 0 ),
           _max_its( 10 ),
           _tag( tag ),
           _comm( NULL )
      {
         set_comm( comm );
      }

      void
      indexer::set_comm( const mpi::comm& comm )
      {
         _comm = &comm;
      }

      void
      indexer::set_max_its( unsigned its )
      {
         _max_its = its;
      }

      unsigned long long
      indexer::request( unsigned long long size )
      {
	 unsigned long long base;
	 if( _comm->size() > 1 )
	 {
	    LOGDLN( "Requesting ", size, " indices." );
	    _comm->send( size, 0, _tag );
	    _comm->recv( base, 0, _tag );
	    LOGDLN( "Received index base: ", base );
	 }
	 else
	 {
	    base = _base;
	    _base += size;
	 }
         return base;
      }

      void
      indexer::master( std::function<void(unsigned long long,unsigned long long)> cb )
      {
	 // Must be the master to run this.
	 ASSERT( _comm->rank() == 0 );

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

      //    class indexer_iterator
      // 	 : public boost::iterator_facade< indexer_iterator,
      // 					  range<unsigned long long>,
      // 					  std::forward_traversal_tag,
      // 					  range<unsigned long long> >
      //    {
      // 	 friend class boost::iterator_core_access;

      //    public:

      // 	 typedef boost::iterator_facade<indexer_iterator,
      // 					range<unsigned long long>,
      // 					std::forward_traversal_tag,
      // 					range<unsigned long long>> super_type;
      // 	 typedef range<unsigned long long> value_type;
      // 	 typedef typename super_type::difference_type difference_type;

      // public:

      //    indexer_iterator( indexer* idxr )
      //       : _idxr( idxr )
      //    {
      //    }

      //    po2_ring_buffer_iterator( const po2_ring_buffer_iterator& src )
      //       : _buf( src._buf ),
      //         _idx( src._idx )
      //    {
      //    }

      // protected:

      //    void
      //    increment()
      //    {
      //       ++_idx;
      //    }

      //    void
      //    decrement()
      //    {
      //       --_idx;
      //    }

      //    void
      //    advance( size_t elems )
      //    {
      //       _idx += elems;
      //    }

      //    bool
      //    equal( const po2_ring_buffer_iterator& op ) const
      //    {
      //       return _buf.norm( _idx ) == _buf.norm( op._idx );
      //    }

      //    const value_type&
      //    dereference() const
      //    {
      //       return _buf[_idx];
      //    }

      //    difference_type
      //    distance_to( const po2_ring_buffer_iterator& op ) const
      //    {
      //       return op._idx - _idx;
      //    }

      // protected:

      //    const po2_ring_buffer<value_type>& _buf;
      //    size_type _idx;
      // };

   }
}
