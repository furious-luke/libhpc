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

#ifndef libhpc_containers_po2_ring_buffer_hh
#define libhpc_containers_po2_ring_buffer_hh

#include <boost/iterator/iterator_facade.hpp>
#include "libhpc/debug/debug.hh"
#include "libhpc/memory/memory.hh"
#include "vector.hh"
#include "functors.hh"
#include "mymath.hh"

class po2_ring_buffer_suite;

namespace hpc {

   template< class T >
   class po2_ring_buffer_iterator;

   ///
   ///
   ///
   template< class T >
   class po2_ring_buffer
   {
      friend class ::po2_ring_buffer_suite;

   public:

      typedef T value_type;
      typedef size_t size_type;
      typedef po2_ring_buffer_iterator<T> iterator;

   public:

      po2_ring_buffer()
      {
      }

      ~po2_ring_buffer()
      {
      }

      void
      resize( size_type size  )
      {
         ASSERT( pow2i( log2i( size ) ) == size, "Must be power of 2 size." );
         _buf.reallocate( size );
         _setup_mask( size );
         reset();
      }

      void
      reset()
      {
         _start = 0;
         _size = 0;
      }

      size_type
      begin() const
      {
         return _start;
      }

      size_type
      end() const
      {
         return _norm( _start + _size );
      }

      template< class Iterator >
      size_type
      insert( Iterator start,
              const Iterator& finish )
      {
         size_type rem = _buf.size() - _size;
         unsigned ii = 0;
         for( ; ii < rem && start != finish; ++ii )
            _buf[_norm( _size + ii )] = *start++;
         _size += ii;

         return ii;
      }

      void
      consume( size_type size )
      {
         ASSERT( size <= _size );
         _start = _norm( _start + size );
         _size -= size;
      }

      value_type&
      operator[]( size_type idx )
      {
         return _buf[_norm( idx )];
      }

      const value_type&
      operator[]( size_type idx ) const
      {
         return _buf[_norm( idx )];
      }

      friend std::ostream&
      operator<<( std::ostream& strm,
		  const po2_ring_buffer& obj )
      {
	 strm << "[";
	 if( obj._size ) {
            strm << obj[0];
            for( unsigned ii = 1; ii < obj._size; ++ii )
               strm << ", " << obj[ii];
	 }
	 strm << "]";
	 return strm;
      }

   protected:

      size_t
      _norm( size_t idx ) const
      {
         return idx & _mask;
      }

      void
      _setup_mask( size_t size )
      {
         unsigned num_bits = log2i( size );
         _mask = 0;
         for( unsigned ii = 0; ii < num_bits; ++ii )
            _mask |= (1 << ii);
      }

   protected:

      vector<value_type> _buf;
      size_type _start;
      size_type _size;
      size_type _mask;
   };

   ///
   ///
   ///
   template< class T >
   class po2_ring_buffer_iterator
      : public boost::iterator_facade< po2_ring_buffer_iterator<T>,
                                       T,
				       std::random_access_iterator_tag,
                                       const T >
   {
      friend class boost::iterator_core_access;

   public:

      typedef T value_type;
      typedef typename po2_ring_buffer<T>::size_type size_type;

   public:

      po2_ring_buffer_iterator( po2_ring_buffer<value_type>& buffer,
                                size_type idx )
         : _buf( buffer ),
           _idx( idx )
      {
      }

   protected:

      void
      increment()
      {
         ++_idx;
      }

      bool
      equal( const po2_ring_buffer_iterator& op ) const
      {
         return _idx == op._idx;
      }

      const value_type&
      dereference() const
      {
         return _buf[_idx];
      }

   protected:

      po2_ring_buffer<value_type>& _buf;
      size_type _idx;
   };
};

#endif