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

#ifndef hpc_containers_po2_ring_buffer_hh
#define hpc_containers_po2_ring_buffer_hh

#include <boost/iterator/iterator_facade.hpp>
#include "libhpc/debug/assert.hh"
#include "libhpc/system/math.hh"
#include "libhpc/system/view.hh"

namespace hpc {

   template< class T >
   class po2_ring_buffer_iterator;

   ///
   ///
   ///
   template< class T >
   class po2_ring_buffer
   {
   public:

      typedef T value_type;
      typedef size_t size_type;
      typedef po2_ring_buffer_iterator<T> iterator;

   public:

      po2_ring_buffer()
      {
      }

      po2_ring_buffer( size_type size )
      {
         resize( size );
      }

      ~po2_ring_buffer()
      {
      }

      void
      resize( size_type size  )
      {
         ASSERT( size == 0 || pow2i( log2i( size ) ) == size, "Must be power of 2 size." );
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

      iterator
      begin() const
      {
         return iterator( *this, 0 );
      }

      iterator
      end() const
      {
         return iterator( *this, _size );
      }

      template< class Iterator >
      size_type
      insert( Iterator start,
              const Iterator& finish )
      {
         size_type rem = _buf.size() - _size;
         unsigned ii = 0;
         for( ; ii < rem && start != finish; ++ii )
            _buf[norm( _start + _size + ii )] = *start++;
         _size += ii;

         return ii;
      }

      void
      insert( value_type value )
      {
         ASSERT( _buf.size() - _size );
         _buf[norm( _start + _size++ )] = value;
      }

      void
      extend( size_type size = 1 )
      {
         ASSERT( _size + size <= _buf.size() );
         _size += size;
      }

      ///
      /// Consume some/all of buffer.
      ///
      /// Can request consuming of more than is in the buffer. This
      /// will consume everything in the buffer, returning how much
      /// was consumed.
      ///
      size_t
      consume( size_type size = 1 )
      {
         size = std::min( size, _size );
         _start = norm( _start + size );
         _size -= size;
         return size;
      }

      value_type
      pop()
      {
         ASSERT( _size, "Insufficient space in ring buffer." );
         size_type pos = norm( _start );
         consume();
         return _buf[pos];
      }

      typename hpc::view<std::vector<value_type> >
      first_vacant_chunk() const
      {
         size_t start = norm( _start + _size );
         size_t size;
         if( start < _start )
            size = _start - start;
         else
            size = _buf.size() - (_start + _size);
         return typename hpc::view<std::vector<value_type> >( _buf, size, start );
      }

      typename hpc::view<std::vector<value_type> >
      second_vacant_chunk() const
      {
         size_t start = norm( _start + _size );
         size_t size;
         if( start < _start )
            size = 0;
         else
            size = _start;
         return typename hpc::view<std::vector<value_type> >( _buf, size );
      }

      size_t
      norm( size_t idx ) const
      {
         return idx & _mask;
      }

      bool
      empty() const
      {
         return _size == 0;
      }

      size_t
      size() const
      {
         return _size;
      }

      size_t
      max_size() const
      {
         return _buf.size();
      }

      size_t
      vacant() const
      {
         return _buf.size() - _size;
      }

      value_type&
      operator[]( size_type idx )
      {
         return _buf[norm( _start + idx )];
      }

      const value_type&
      operator[]( size_type idx ) const
      {
         return _buf[norm( _start + idx )];
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

      void
      _setup_mask( size_t size )
      {
         _mask = 0;
         if( size > 0 )
         {
            unsigned num_bits = log2i( size );
            for( unsigned ii = 0; ii < num_bits; ++ii )
               _mask |= (1 << ii);
         }
      }

   protected:

      std::vector<value_type> _buf;
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

      typedef boost::iterator_facade<po2_ring_buffer_iterator<T>,
                                     T,
				     std::random_access_iterator_tag,
                                     const T> super_type;
      typedef T value_type;
      typedef typename po2_ring_buffer<T>::size_type size_type;
      typedef typename super_type::difference_type difference_type;

   public:

      po2_ring_buffer_iterator( const po2_ring_buffer<value_type>& buffer,
                                size_type idx )
         : _buf( buffer ),
           _idx( idx )
      {
      }

      po2_ring_buffer_iterator( const po2_ring_buffer_iterator& src )
         : _buf( src._buf ),
           _idx( src._idx )
      {
      }

   protected:

      void
      increment()
      {
         ++_idx;
      }

      void
      decrement()
      {
         --_idx;
      }

      void
      advance( size_t elems )
      {
         _idx += elems;
      }

      bool
      equal( const po2_ring_buffer_iterator& op ) const
      {
         return _buf.norm( _idx ) == _buf.norm( op._idx );
      }

      const value_type&
      dereference() const
      {
         return _buf[_idx];
      }

      difference_type
      distance_to( const po2_ring_buffer_iterator& op ) const
      {
         return op._idx - _idx;
      }

   protected:

      const po2_ring_buffer<value_type>& _buf;
      size_type _idx;
   };

}

#endif
