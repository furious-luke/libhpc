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

#ifndef containers_fibre_hh
#define containers_fibre_hh

#include <stddef.h>
#include "libhpc/debug/checks.hh"
#include "vector.hh"

namespace hpc {

   template< class T >
   class const_fibre_iterator;

   ///
   ///
   ///
   template< class T >
   class fibre_iterator
      : public std::iterator< std::random_access_iterator_tag,
                              vector<T>,
                              ptrdiff_t,
                              T*,
                              typename vector<T>::view >
   {
   public:

      typedef ptrdiff_t difference_type;

      fibre_iterator()
         : _ptr( 0 ),
           _fibre_size( 0 )
      {
      }

      fibre_iterator( const T* ptr, index fibre_size )
         : _ptr( (T*)ptr ),
           _fibre_size( fibre_size )
      {
      }

      fibre_iterator( typename vector<T>::iterator vit, index fibre_size )
         : _ptr( vit.base() ),
           _fibre_size( fibre_size )
      {
      }

      fibre_iterator( const fibre_iterator& it )
         : _ptr( it._ptr ),
           _fibre_size( it._fibre_size )
      {
      }

      fibre_iterator&
      operator++()
      {
         this->_ptr += this->_fibre_size;
         return *this;
      }

      fibre_iterator
      operator++( int )
      {
         this->_ptr += this->_fibre_size;
         return fibre_iterator( this->_ptr, this->_fibre_size );
      }

      fibre_iterator&
      operator--()
      {
         this->_ptr -= this->_fibre_size;
         return *this;
      }

      fibre_iterator
      operator--( int )
      {
         this->_ptr -= this->_fibre_size;
         return fibre_iterator( this->_ptr, this->_fibre_size );
      }

      fibre_iterator&
      operator-=( difference_type op )
      {
         this->_ptr -= op*this->_fibre_size;
         return *this;
      }

      fibre_iterator
      operator+( difference_type op )
      {
         return fibre_iterator( this->_ptr + op*this->_fibre_size, this->_fibre_size );
      }

      fibre_iterator&
      operator+=( difference_type op )
      {
         this->_ptr += op*this->_fibre_size;
         return *this;
      }

      fibre_iterator
      operator-( difference_type op )
      {
         return fibre_iterator( this->_ptr - op*this->_fibre_size, this->_fibre_size );
      }

      difference_type
      operator-( fibre_iterator op )
      {
         ASSERT( this->_fibre_size == op._fibre_size );
         ASSERT( (this->_ptr - op._ptr)%this->_fibre_size == 0 );
         return (this->_ptr - op._ptr)/this->_fibre_size;
      }

      difference_type
      operator-( const_fibre_iterator<T> op )
      {
         ASSERT( this->_fibre_size == op._fibre_size );
         ASSERT( (this->_ptr - op._ptr)%this->_fibre_size == 0 );
         return (this->_ptr - op._ptr)/this->_fibre_size;
      }

      bool
      operator==( const fibre_iterator& op ) const
      {
         return this->_ptr == op._ptr;
      }

      bool
      operator!=( const fibre_iterator& op ) const
      {
         return !this->operator==( op );
      }

      typename vector<T>::view
      operator*()
      {
         return typename vector<T>::view( this->_ptr, this->_fibre_size );
      }

      typename vector<T>::view
      operator[]( hpc::index idx )
      {
         return typename vector<T>::view( this->_ptr + idx*this->_fibre_size, this->_fibre_size );
      }

      bool
      operator<( const fibre_iterator& op ) const
      {
         return this->_ptr < op._ptr;
      }

      bool
      operator>( const fibre_iterator& op ) const
      {
         return this->_ptr > op._ptr;
      }

      bool
      operator<=( const fibre_iterator& op ) const
      {
         return this->_ptr <= op._ptr;
      }

      bool
      operator>=( const fibre_iterator& op ) const
      {
         return this->_ptr >= op._ptr;
      }

      T*
      base() const
      {
         return this->_ptr;
      }

      index
      fibre_size() const
      {
         return this->_fibre_size;
      }

   private:

      T* _ptr;
      index _fibre_size;

      friend class const_fibre_iterator<T>;
   };

   ///
   ///
   ///
   template< class T >
   class const_fibre_iterator
      : public std::iterator< std::random_access_iterator_tag,
                              vector<T>,
                              ptrdiff_t,
                              const T*,
                              const typename vector<T>::view >
   {
   public:

      typedef ptrdiff_t difference_type;

      const_fibre_iterator()
         : _ptr( 0 ),
           _fibre_size( 0 )
      {
      }

      const_fibre_iterator( const T* ptr, index fibre_size )
         : _ptr( ptr ),
           _fibre_size( fibre_size )
      {
      }

      const_fibre_iterator( const typename vector<T>::iterator vit, index fibre_size )
         : _ptr( vit.base() ),
           _fibre_size( fibre_size )
      {
      }

      const_fibre_iterator( const typename vector<T>::const_iterator vit, index fibre_size )
         : _ptr( vit.base() ),
           _fibre_size( fibre_size )
      {
      }

      const_fibre_iterator( const const_fibre_iterator& it )
         : _ptr( it._ptr ),
           _fibre_size( it._fibre_size )
      {
      }

      const_fibre_iterator( const fibre_iterator<T>& it )
         : _ptr( it._ptr ),
           _fibre_size( it._fibre_size )
      {
      }

      const_fibre_iterator&
      operator++()
      {
         this->_ptr += this->_fibre_size;
         return *this;
      }

      const_fibre_iterator
      operator++( int )
      {
         this->_ptr += this->_fibre_size;
         return const_fibre_iterator( this->_ptr, this->_fibre_size );
      }

      const_fibre_iterator&
      operator+=( difference_type op )
      {
         this->_ptr += op*this->_fibre_size;
         return *this;
      }

      const_fibre_iterator&
      operator--()
      {
         this->_ptr -= this->_fibre_size;
         return *this;
      }

      const_fibre_iterator
      operator--( int )
      {
         this->_ptr -= this->_fibre_size;
         return const_fibre_iterator( this->_ptr, this->_fibre_size );
      }

      const_fibre_iterator&
      operator-=( difference_type op )
      {
         this->_ptr -= op*this->_fibre_size;
         return *this;
      }

      const_fibre_iterator
      operator+( difference_type op )
      {
         return const_fibre_iterator( this->_ptr + op*this->_fibre_size, this->_fibre_size );
      }

      const_fibre_iterator
      operator-( difference_type op )
      {
         return const_fibre_iterator( this->_ptr - op*this->_fibre_size, this->_fibre_size );
      }

      const_fibre_iterator
      operator+( const_fibre_iterator op )
      {
         return const_fibre_iterator( this->_ptr + op*this->_fibre_size, this->_fibre_size );
      }

      difference_type
      operator-( const_fibre_iterator op )
      {
         ASSERT( this->_fibre_size == op._fibre_size );
         ASSERT( (this->_ptr - op._ptr)%this->_fibre_size == 0 );
         return (this->_ptr - op._ptr)/this->_fibre_size;
      }

      difference_type
      operator-( fibre_iterator<T> op )
      {
         ASSERT( this->_fibre_size == op._fibre_size );
         ASSERT( (this->_ptr - op._ptr)%this->_fibre_size == 0 );
         return (this->_ptr - op._ptr)/this->_fibre_size;
      }

      bool
      operator==( const const_fibre_iterator& op ) const
      {
         return this->_ptr == op._ptr;
      }

      bool
      operator!=( const const_fibre_iterator& op ) const
      {
         return !this->operator==( op );
      }

      const typename vector<T>::view
      operator*() const
      {
         return typename vector<T>::view( this->_ptr, this->_fibre_size );
      }

      typename vector<T>::view
      operator[]( hpc::index idx ) const
      {
         return typename vector<T>::view( this->_ptr + idx*this->_fibre_size, this->_fibre_size );
      }

      bool
      operator<( const const_fibre_iterator& op ) const
      {
         return this->_ptr < op._ptr;
      }

      bool
      operator>( const const_fibre_iterator& op ) const
      {
         return this->_ptr > op._ptr;
      }

      bool
      operator<=( const const_fibre_iterator& op ) const
      {
         return this->_ptr <= op._ptr;
      }

      bool
      operator>=( const const_fibre_iterator& op ) const
      {
         return this->_ptr >= op._ptr;
      }

      const T*
      base() const
      {
         return this->_ptr;
      }

      index
      fibre_size() const
      {
         return this->_fibre_size;
      }

   private:

      const T* _ptr;
      index _fibre_size;

      friend class fibre_iterator<T>;
   };

   ///
   ///
   ///
   template< class T >
   class fibre
      : public vector<T>
   {
   public:

      typedef vector<T>                     super_type;
      typedef T                             value_type;
      typedef typename vector<T>::size_type size_type;
      typedef fibre_iterator<T>             iterator;
      typedef const_fibre_iterator<T>       const_iterator;

      fibre()
	 : vector<T>(),
	   _num_fibres( 0 ),
	   _fibre_size( 0 )
      {
      }

      fibre( index fibre_size,
             index size = 0 )
         : vector<T>(),
           _num_fibres( 0 ),
           _fibre_size( fibre_size )
      {
         this->resize( size );
      }

      ~fibre()
      {
      }

      void
      set_fibre_size( size_type size )
      {
	 this->deallocate();
	 this->_num_fibres = 0;
	 this->_fibre_size = size;
      }

      void
      take( fibre<value_type>& src )
      {
         super_type::take( src );
         _num_fibres = src._num_fibres;
         _fibre_size = src._fibre_size;
         src._num_fibres = 0;
         src._fibre_size = 0;
      }

      size_type
      fibre_size() const
      {
	 return this->_fibre_size;
      }

      size_type
      size() const
      {
	 return this->_num_fibres;
      }

      size_type
      max_size() const
      {
	 CHECK(this->check_sizes());
	 return vector<T>::max_size()/this->_fibre_size;
      }

      size_type
      capacity() const
      {
	 CHECK(this->check_sizes());
	 return vector<T>::capacity()/this->_fibre_size;
      }

      void
      reserve( size_type size )
      {
	 CHECK(this->check_sizes());
	 vector<T>::reserve(size*this->_fibre_size);
      }

      void
      resize( size_type size )
      {
	 // CHECK(this->check_sizes());
	 vector<T>::resize(size*this->_fibre_size);
	 this->_num_fibres = size;
      }

      void
      reallocate( size_type size )
      {
	 // CHECK(this->check_sizes());
	 vector<T>::reallocate(size*this->_fibre_size);
	 this->_num_fibres = size;
      }

      void
      reallocate( size_type fibre_size,
                  size_type size )
      {
         set_fibre_size( fibre_size );
         reallocate( size );
      }

      void
      clear()
      {
	 vector<T>::clear();
	 this->_num_fibres = 0;
      }

      void
      deallocate()
      {
	 this->clear();
	 vector<T>().swap(*this);
      }

      const_iterator
      begin() const
      {
         return const_iterator( vector<T>::begin(), this->_fibre_size );
      }

      iterator
      begin()
      {
         return iterator( vector<T>::begin(), this->_fibre_size );
      }

      const_iterator
      end() const
      {
         return const_iterator( vector<T>::end(), this->_fibre_size );
      }

      iterator
      end()
      {
         return iterator( vector<T>::end(), this->_fibre_size );
      }

      typename vector<T>::iterator
      vbegin()
      {
         return vector<T>::begin();
      }

      typename vector<T>::iterator
      vend()
      {
         return vector<T>::end();
      }

      const typename vector<T>::view
      front() const
      {
         return (*this)[0];
      }

      const typename vector<T>::view
      back() const
      {
         return (*this)[_num_fibres - 1];
      }

      const typename vector<T>::view
      operator[](index idx) const
      {
	 ASSERT(idx >= 0 && idx < this->_num_fibres, "Index out of bounds.");
	 return typename vector<T>::view(*this, this->_fibre_size, idx*this->_fibre_size);
      }

      typename vector<T>::view
      operator[](index idx)
      {
	 ASSERT(idx >= 0 && idx < this->_num_fibres, "Index out of bounds.");
	 return typename vector<T>::view(*this, this->_fibre_size, idx*this->_fibre_size);
      }

      T
      operator()( index idx,
		  index fibre_idx ) const
      {
	 ASSERT( fibre_idx < this->_fibre_size );
	 return vector<T>::operator[]( idx*this->_fibre_size + fibre_idx );
      }

      T&
      operator()( index idx,
		  index fibre_idx )
      {
	 ASSERT( fibre_idx < this->_fibre_size );
	 return vector<T>::operator[]( idx*this->_fibre_size + fibre_idx );
      }

      friend std::ostream&
      operator<<( std::ostream& strm,
		  const fibre<T>& obj )
      {
	 strm << "[";
	 if(obj.size()) {
	    strm << "(" << obj(0, 0);
	    for(index jj = 1; jj < obj._fibre_size; ++jj)
	       strm << ", " << obj(0, jj);
	    strm << ")";
	    for(index ii = 1; ii < obj.size(); ++ii) {
	       strm << ", (" << obj(ii, 0);
	       for(index jj = 1; jj < obj._fibre_size; ++jj)
		  strm << ", " << obj(ii, jj);
	       strm << ")";
	    }
	 }
	 strm << "]";
	 return strm;
      }

   protected:

#ifndef NDEBUG
      void
      check_sizes() const
      {
	 ASSERT(this->_fibre_size > 0, "Fibre size must be greater than zero.");
	 ASSERT(vector<T>::size()%this->_fibre_size == 0, "Bad number of fibres.");
	 ASSERT(vector<T>::size()/this->_fibre_size == this->size(), "Number of fibres mismatch.");
      }
#endif

   private:

      size_type _num_fibres;
      size_type _fibre_size;
   };

   template< class T >
   fibre_iterator<T>
   operator+( ptrdiff_t op_a, const fibre_iterator<T>& op_b )
   {
      return fibre_iterator<T>( op_b.base() + op_a*op_b.fibre_size(), op_b.fibre_size() );
   }

   template< class T >
   const_fibre_iterator<T>
   operator+( ptrdiff_t op_a, const const_fibre_iterator<T>& op_b )
   {
      return const_fibre_iterator<T>( op_b.base() + op_a*op_b.fibre_size(), op_b.fibre_size() );
   }
}

#endif
