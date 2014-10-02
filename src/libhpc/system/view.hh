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

#ifndef hpc_containers_view_hh
#define hpc_containers_view_hh

#include <vector>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include "libhpc/debug/assert.hh"
#include "libhpc/system/type_traits.hh"
#include "libhpc/system/stream.hh"

namespace hpc {

   template< class > class view_iterator;

   template< class Vector >
   class const_view
   {
   public:

      typedef typename boost::remove_const<Vector>::type const vector_type;
      typedef size_t size_type;
      typedef size_t key_type;

      typedef typename Vector::value_type      mapped_type;
      typedef typename Vector::value_type      value_type;
      typedef typename Vector::const_pointer   const_pointer;
      typedef typename Vector::pointer         pointer;
      typedef typename Vector::const_reference const_element_reference;

      typedef view_iterator<value_type const> const_iterator;

   public:

      const_view()
         : _ptr( 0 ),
           _size( 0 )
      {
      }

      template< class Other,
                typename boost::enable_if<boost::is_base_of<vector_type,Other>,int>::type = 0 >
      const_view( Other const& vec )
         : _ptr( (pointer)vec.data() ),
           _size( vec.size() )
      {
      }

      template< class Other,
                typename boost::enable_if<boost::is_base_of<vector_type,Other>,int>::type = 0 >
      const_view( Other const& vec,
                  size_t size,
                  size_t offs = 0 )
         : _ptr( (pointer)vec.data() + offs ),
           _size( size )
      {
         ASSERT( size >= 0 );
         ASSERT( offs >= 0 );
         ASSERT( offs + size <= vec.size() );
      }

      const_view( const_pointer ptr,
                  size_t size,
                  size_t offs = 0 )
         : _ptr( (pointer)ptr + offs ),
           _size( size )
      {
      }

      void
      assign( const_pointer ptr,
              size_t size = 0 )
      {
	 _ptr = (pointer)ptr;
	 _size = size;
      }

      template< class Other,
                typename boost::enable_if<boost::is_base_of<vector_type,Other>,int>::type = 0 >
      void
      assign( Other const& op,
              size_t size = 0,
              size_t offs = 0 )
      {
	 ASSERT( offs + size <= op.size() );
	 _ptr = (pointer)op.data() + offs;
	 _size = size ? size : op.size();
      }

      // template< class Other >
      // view&
      // assign( Other const& op,
      //         typename boost::enable_if<boost::is_base_of<vector_type,Other>,int>::type = 0 )
      // {
      //    ASSERT( op.size() == this->_size );
      //    std::copy( op.begin(), op.end(), begin() );
      //    return *this;
      // }

      void
      shrink( size_t size,
              size_t offs = 0 )
      {
         ASSERT( size + offs <= _size );
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

      const_pointer const
      data() const
      {
         return _ptr;
      }

      const_iterator
      begin() const
      {
         return const_iterator( _ptr );
      }

      const_iterator
      end() const
      {
         return const_iterator( _ptr + _size );
      }

      const_element_reference
      front() const
      {
         ASSERT( _size );
	 return *begin();
      }

      const_element_reference
      back() const
      {
         ASSERT( _size );
	 return *(end() - 1);
      }

      const_element_reference
      operator[]( size_t idx ) const
      {
         ASSERT( idx < _size, "Index out of bounds." );
         return _ptr[idx];
      }

      template< class Other,
                typename boost::enable_if<boost::is_base_of<vector_type,Other>,int>::type = 0 >
      bool
      compare( Other const& op ) const
      {
         if( _size != op._size )
            return false;
         typename Other::const_iterator op_it = op.begin();
         for( const_iterator it = begin(); it != end(); ++it, ++op_it )
         {
            if( *it != *op_it )
               return false;
         }
         return true;
      }

      template< class Other >
      bool
      operator==( Other const& op )
      {
         return compare<Other>( op );
      }

      operator const_pointer() const
      {
	 return _ptr;
      }

   protected:

      pointer _ptr;
      size_type _size;
   };

   template< class Vector,
             class Enable = void >
   class view;

   template< class Vector >
   class view< Vector,
               typename boost::enable_if<boost::is_const<Vector> >::type >
      : public const_view<Vector>
   {
   public:

      typedef const_view<Vector> super_type;
      typedef Vector vector_type;
      typedef size_t size_type;
      typedef size_t key_type;

      typedef typename Vector::value_type      mapped_type;
      typedef typename Vector::value_type      value_type;
      typedef typename Vector::const_pointer   const_pointer;
      typedef typename Vector::pointer         pointer;
      typedef typename Vector::const_reference const_element_reference;
      typedef typename Vector::reference       element_reference;

      typedef view_iterator<value_type>       iterator;
      typedef view_iterator<value_type const> const_iterator;

   public:

      view()
         : super_type()
      {
      }

      template< class Other,
                typename boost::enable_if<boost::is_base_of<vector_type,Other>,int>::type = 0 >
      view( Other const& vec )
         : super_type( vec )
      {
      }

      template< class Other,
                typename boost::enable_if<boost::is_base_of<vector_type,Other>,int>::type = 0 >
      view( Other const& vec,
            size_t size,
            size_t offs = 0 )
         : super_type( vec, size, offs )
      {
      }

      view( const_pointer ptr,
            size_t size,
            size_t offs = 0 )
         : super_type( ptr, size, offs )
      {
      }
   };

   template< class Vector >
   class view< Vector,
               typename boost::disable_if<boost::is_const<Vector> >::type >
      : public const_view<Vector>
   {
   public:

      typedef const_view<Vector> super_type;
      typedef Vector vector_type;
      typedef size_t size_type;
      typedef size_t key_type;

      typedef typename Vector::value_type      mapped_type;
      typedef typename Vector::value_type      value_type;
      typedef typename Vector::const_pointer   const_pointer;
      typedef typename Vector::pointer         pointer;
      typedef typename Vector::const_reference const_element_reference;
      typedef typename Vector::reference       element_reference;

      typedef view_iterator<value_type>       iterator;
      typedef view_iterator<value_type const> const_iterator;

   public:

      view()
         : super_type()
      {
      }

      template< class Other,
                typename boost::enable_if<boost::is_base_of<vector_type,Other>,int>::type = 0 >
      view( Other const& vec )
         : super_type( vec )
      {
      }

      template< class Other,
                typename boost::enable_if<boost::is_base_of<vector_type,Other>,int>::type = 0 >
      view( Other const& vec,
            size_t size,
            size_t offs = 0 )
         : super_type( vec, size, offs )
      {
      }

      view( const_pointer ptr,
            size_t size,
            size_t offs = 0 )
         : super_type( ptr, size, offs )
      {
      }

      const_pointer
      data() const
      {
         return this->_ptr;
      }

      pointer
      data()
      {
         return this->_ptr;
      }

      const_iterator
      begin() const
      {
         return const_iterator( this->_ptr );
      }

      iterator
      begin()
      {
         return iterator( this->_ptr );
      }

      const_iterator
      end() const
      {
         return const_iterator( this->_ptr + this->_size );
      }

      iterator
      end()
      {
         return iterator( this->_ptr + this->_size );
      }

      const_element_reference
      operator[]( size_t idx ) const
      {
         ASSERT( idx < this->_size, "Index out of bounds." );
         return this->_ptr[idx];
      }

      element_reference
      operator[]( size_t idx )
      {
         ASSERT( idx < this->_size, "Index out of bounds." );
         return this->_ptr[idx];
      }

      template< class Other >
      view&
      operator=( Other const& op )
      {
         this->template assign<Other>( op );
         return *this;
      }

      operator pointer()
      {
	 return this->_ptr;
      }

      operator const pointer() const
      {
	 return this->_ptr;
      }
   };

   template< class T >
   class view_iterator
      : public boost::iterator_facade< view_iterator<T>,
                                       T,
				       std::random_access_iterator_tag >
   {
      friend class boost::iterator_core_access;
      // template< class > friend class view_iterator;

   public:

      explicit view_iterator( T* ptr = 0 )
         : _ptr( ptr )
      {
      }

      template< class U >
      view_iterator( const view_iterator<U>& src )
         : _ptr( src._ptr )
      {
      }

   protected:

      void
      increment()
      {
         ++_ptr;
      }

      void
      decrement()
      {
         --_ptr;
      }

      template< class U >
      bool
      equal( const view_iterator<U>& op ) const
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
      distance_to( const view_iterator<U>& op ) const
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
   std::ostream&
   operator<<( std::ostream& strm,
               const_view<Vector> const& obj )
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

   template< class Vector >
   struct type_traits< view<Vector> >
   {
      typedef       view<Vector>  value;
      typedef const view<Vector>  const_value;
      typedef       view<Vector>  reference;
      typedef const view<Vector>& const_reference;
   };

   template< class Vector >
   struct type_traits< view<Vector>& >
   {
      typedef       view<Vector>  value;
      typedef const view<Vector>  const_value;
      typedef       view<Vector>  reference;
      typedef const view<Vector>& const_reference;
   };

   template< class Vector >
   struct random_access_trait< view<Vector> >
      : boost::true_type {};

}

#endif
