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

#ifndef libhpc_containers_vector_view_hh
#define libhpc_containers_vector_view_hh

#include <boost/iterator/iterator_facade.hpp>
#include "libhpc/debug/assert.hh"
#include "libhpc/system/types.hh"
#include "libhpc/system/type_traits.hh"

namespace hpc {

   template< class > class vector_view_iterator;

   template< class Vector >
   class vector_view 
   {
   public:

      typedef Vector vector_type;
      typedef size_t size_type;
      typedef size_t key_type;
      typedef typename Vector::value_type mapped_type;
      typedef typename Vector::value_type value_type;
      typedef typename Vector::const_pointer const_pointer;
      typedef typename Vector::pointer pointer;
      typedef typename Vector::const_reference const_element_reference;
      typedef typename Vector::reference element_reference;
      typedef vector_view_iterator<value_type> iterator;
      typedef vector_view_iterator<const value_type> const_iterator;

      vector_view()
         : _ptr( 0 ),
           _size( 0 )
      {
      }

      vector_view( const vector_type& vec )
         : _ptr( (pointer)vec.data() ),
           _size( vec.size() )
      {
      }

      vector_view( const Vector& vec,
                   size_t size,
                   size_t offs = 0 )
         : _ptr( (pointer)vec.data() + offs ),
           _size( size )
      {
         ASSERT( size >= 0 );
         ASSERT( offs >= 0 );
         ASSERT( offs + size <= vec.size() );
      }

      vector_view( const vector_view& src )
         : _ptr( (pointer)src.data() ),
           _size( src.size() )
      {
      }

      vector_view( const vector_view& view,
		   size_t size,
		   size_t offs = 0 )
         : _ptr( (pointer)view.data() + offs ),
           _size( size )
      {
	 ASSERT( size >= 0 );
	 ASSERT( offs >= 0 );
	 ASSERT( offs + size <= view.size() );
      }

      vector_view( const_pointer ptr,
                   size_t size,
                   size_t offs = 0 )
      {
         _ptr = (pointer)ptr + offs;
         _size = size;
      }

      void
      assign( const vector_view& op,
              size_t size,
              size_t offs = 0 )
      {
	 ASSERT( size >= 0 );
	 ASSERT( offs >= 0 );
	 ASSERT( offs + size <= op.size() );
	 _ptr = (pointer)op.data() + offs;
	 _size = size;
      }

      void
      shrink( size_t size,
              size_t offs = 0 )
      {
         ASSERT( size + offs <= this->_size );
         _ptr += offs;
         _size = size;
      }

      size_type
      size() const
      {
         return _size;
      }

      bool
      empty() const
      {
         return _size == 0;
      }

      const pointer
      data() const
      {
         return _ptr;
      }

      const_iterator
      begin() const
      {
         return const_iterator( _ptr );
      }

      iterator
      begin()
      {
         return iterator( _ptr );
      }

      const_iterator
      end() const
      {
         return const_iterator( _ptr + _size );
      }

      iterator
      end()
      {
         return iterator( _ptr + _size );
      }

      const_element_reference
      front() const
      {
	 return *this->begin();
      }

      const_element_reference
      back() const
      {
	 return *(this->end() - 1);
      }

      const_element_reference
      operator[]( size_t idx ) const
      {
         ASSERT( idx < _size, "Index out of bounds." );
         return _ptr[idx];
      }

      element_reference
      operator[]( size_t idx )
      {
         ASSERT( idx < _size, "Index out of bounds." );
         return _ptr[idx];
      }

      vector_view&
      operator=( const vector_view& op )
      {
         _ptr = op._ptr;
         _size = op._size;
      }

      bool
      operator==( const vector_view& op ) const
      {
         if( _size != op._size )
            return false;
         for( size_t ii = 0; ii < _size; ++ii )
         {
            if( _ptr[ii] != op._ptr[ii] )
               return false;
         }
         return true;
      }

      operator pointer()
      {
	 return _ptr;
      }

      operator const pointer() const
      {
	 return _ptr;
      }

      friend std::ostream&
      operator<<( std::ostream& strm,
		  const vector_view<Vector>& obj )
      {
	 strm << "[";
	 if( obj.size() )
         {
	    strm << obj[0];
	    for( size_t ii = 1; ii < obj.size(); ++ii )
	       strm << ", " << obj[ii];
	 }
	 strm << "]";
	 return strm;
      }

   protected:

      pointer _ptr;
      size_type _size;
   };

   template< class T >
   class vector_view_iterator
      : public boost::iterator_facade< vector_view_iterator<T>,
                                       T,
				       std::random_access_iterator_tag >
   {
      friend class boost::iterator_core_access;
      // template< class > friend class vector_view_iterator;

   public:

      explicit vector_view_iterator( T* ptr )
         : _ptr( ptr )
      {
      }

      template< class U >
      vector_view_iterator( const vector_view_iterator<U>& src )
         : _ptr( src._ptr )
      {
      }

   protected:

      void
      increment()
      {
         ++_ptr;
      }

      template< class U >
      bool
      equal( const vector_view_iterator<U>& op ) const
      {
         return _ptr == op._ptr;
      }

      T&
      dereference() const
      {
         return *_ptr;
      }

      template< class U >
      size_t
      distance_to( const vector_view_iterator<U>& op ) const
      {
         return op._ptr - _ptr;
      }

      void
      advance( size_t dist )
      {
         _ptr += dist;
      }

   protected:

      T* _ptr;
   };

   template< class Vector >
   struct type_traits< vector_view<Vector> >
   {
      typedef       vector_view<Vector> value;
      typedef const vector_view<Vector> const_value;
      typedef       vector_view<Vector> reference;
      typedef const vector_view<Vector> const_reference;
   };

}

#endif
