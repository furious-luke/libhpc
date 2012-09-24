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

#ifndef libhpc_containers_ring_buffer_hh
#define libhpc_containers_ring_buffer_hh

#include <boost/iterator/iterator_facade.hpp>
#include "libhpc/debug/debug.hh"
#include "libhpc/memory/memory.hh"
#include "vector.hh"
#include "functors.hh"

class ring_buffer_suite;

namespace hpc {

   template< class T >
   class ring_buffer_chunk_iterator;

   ///
   ///
   ///
   template< class T >
   class ring_buffer
   {
      friend class ring_buffer_chunk_iterator<T>;
      friend class ::ring_buffer_suite;

   public:

      typedef T value_type;
      typedef size_t size_type;
      typedef ring_buffer_chunk_iterator<value_type> chunk_iterator;

   public:

      ring_buffer()
      {
      }

      ~ring_buffer()
      {
      }

      void
      resize( size_type size  )
      {
         _buf.reallocate( size );
         reset();
      }

      void
      reset()
      {
         _start = _buf.begin();
         _finish = _buf.begin();
         _size = 0;
      }

      template< class Iterator >
      size_type
      insert( Iterator start,
              const Iterator& finish )
      {
         // If there is nothing to insert, get out of here.
         if( start == finish )
            return 0;

         // Is there any space available?
         if( _finish == _buf.end() )
         {
            if( _start == _buf.begin() )
               return 0;

            // Move finish pointer to start of the buffer.
            _finish = _buf.begin();
         }
         else if( _start == _finish + 1 )
            return 0;

         // Do we have any space between the buffer finish and the
         // hard buffer end?
         size_type cnt = 0;
         if( _start <= _finish )
         {
            size_type max_len = _buf.end() - _finish;
            while( max_len-- && start != finish )
            {
               *_finish++ = *start++;
               ++cnt;
            }
            if( start != finish )
               _finish = _buf.begin();
         }

         // Do we have more to insert and space between finish and start?
         if( start != finish && _finish + 1 < _start )
         {
            size_type max_len = _start - _finish;
            --max_len;
            while( max_len-- && start != finish )
            {
               *_finish++ = *start++;
               ++cnt;
            }
         }

         // Update my current size.
         _size += cnt;

         return cnt;
      }

      void
      consume( size_type size )
      {
         // Is all the data contiguous?
         if( _start < _finish )
         {
            size_type rem = std::min( size, _finish - _start );
            ASSERT( size == rem );
            _start += rem;
         }
         else
         {
            size_type rem = std::min( size, _buf.end() - _start );
            size -= rem;
            if( size )
            {
               rem = std::min( size, _finish - _buf.begin() );
               ASSERT( size == rem );
               _start = _buf.begin() + rem;
            }
            else
               _start += rem;
         }
      }

      chunk_iterator
      chunk_begin()
      {
         return chunk_iterator( _buf );
      }

      chunk_iterator
      chunk_end()
      {
         return chunk_iterator();
      }

      friend std::ostream&
      operator<<( std::ostream& strm,
		  const ring_buffer& obj )
      {
	 strm << "[";
	 if( obj.size() ) {
	    typename ring_buffer<value_type>::const_iterator it = obj.begin();
	    strm << *it;
	    ++it;
	    for( ; it != obj.end(); ++it )
	       strm << ", " << *it;
	 }
	 strm << "]";
	 return strm;
      }

   protected:

      vector<value_type> _buf;
      typename vector<value_type>::iterator _start, _finish;
      unsigned _size;
   };

   ///
   ///
   ///
   template< class T >
   class ring_buffer_chunk_iterator
      : public boost::iterator_facade< ring_buffer_chunk_iterator<T>,
                                       typename vector<T>::view,
				       std::forward_iterator_tag,
                                       typename vector<T>::view >
   {
      friend class boost::iterator_core_access;

   public:

      typedef T value_type;

   public:

      ring_buffer_chunk_iterator( ring_buffer<value_type>& buffer )
         : _buf( buffer )
      {
         _begin();
      }

      ring_buffer_chunk_iterator()
         : _finish( NULL )
      {
      }

   protected:

      void
      _begin()
      {
         _start = _buf._start;
         if( _buf._finish < _buf._start )
            _finish = _buf._buf.end();
         else
            _finish = _buf._finish;
      }

      void
      increment()
      {
         if( _buf._finish < _finish )
         {
            _start = _buf._buf.begin();
            _finish = _buf._finish;
         }
         else
            _finish = NULL;
      }

      bool
      equal( const ring_buffer_chunk_iterator& op ) const
      {
         return _finish == op._finish;
      }

      typename vector<value_type>::view
      dereference() const
      {
         return typename vector<value_type>::view( _buf._buf, _start, _start + _finish );
      }

   protected:

      ring_buffer<value_type>& _buf;
      typename vector<value_type>::iterator _start, _finish;
   };
};

#endif
