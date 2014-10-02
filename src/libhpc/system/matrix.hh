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

#ifndef hpc_containers_matrix_hh
#define hpc_containers_matrix_hh

#include <vector>
#include "view.hh"

namespace hpc {

   template< class > class matrix_rows_iterator;

   template< class T >
   class matrix
   {
   public:

      typedef typename std::vector<T>::size_type size_type;
      typedef          T                         value_type;

      typedef matrix_rows_iterator<std::vector<T> >      rows_iterator;
      typedef matrix_rows_iterator<std::vector<T> const> const_rows_iterator;

   public:

      matrix()
      {
      }

      matrix( size_type n_rows,
	      size_type n_cols )
	 : _vec( n_rows*n_cols ),
	   _size{ n_rows, n_cols }
      {
      }

      void
      resize( size_type n_rows,
              size_type n_cols )
      {
         _vec.resize( n_rows*n_cols );
         _size[0] = n_rows;
         _size[1] = n_cols;
      }

      size_type
      n_rows() const
      {
         return _size[0];
      }

      size_type
      n_cols() const
      {
         return _size[1];
      }

      view<std::vector<value_type> >
      operator[]( size_type row )
      {
         ASSERT( row < _size[0], "Matrix row out of bounds." );
         return view<std::vector<value_type> >( _vec, _size[1], row*_size[1] );
      }

      view<std::vector<value_type> const>
      operator[]( size_type row ) const
      {
         ASSERT( row < _size[0], "Matrix row out of bounds." );
         return view<std::vector<value_type> const>( _vec, _size[1], row*_size[1] );
      }

      value_type&
      operator()( size_type row,
                  size_type col )
      {
         ASSERT( row < _size[0], "Matrix row out of bounds." );
         ASSERT( col < _size[1], "Matrix column out of bounds." );
         return _vec[row*_size[1] + col];
      }

      value_type const&
      operator()( size_type row,
                  size_type col ) const
      {
         return (*((matrix*)this))( row, col );
      }

      const_rows_iterator
      rows_begin( size_t row = 0 ) const
      {
         return const_rows_iterator( _vec.data() + row*_size[1], _size[1] );
      }

      const_rows_iterator
      rows_end() const
      {
         return const_rows_iterator( _vec.data() + _vec.size(), _size[1] );
      }

      const_rows_iterator
      rows_end( size_t row ) const
      {
         return const_rows_iterator( _vec.data() + row*_size[1], _size[1] );
      }

   protected:

      std::vector<T> _vec;
      std::array<size_type,2> _size;
   };

   template< class T >
   class matrix_rows_iterator
      : public boost::iterator_facade< matrix_rows_iterator<T>,
                                       hpc::view<T>,
				       std::random_access_iterator_tag,
                                       hpc::view<T> >
   {
      friend class boost::iterator_core_access;

   public:

      typedef hpc::view<T>                  reference_type;
      typedef hpc::view<T>                  value_type;
      typedef typename T::value_type*       pointer_type;
      typedef typename T::value_type const* const_pointer_type;

   public:

      explicit matrix_rows_iterator( const_pointer_type ptr = 0,
                                     size_t row_size = 0 )
         : _ptr( (pointer_type)ptr ),
           _row_size( row_size )
      {
      }

      template< class U >
      matrix_rows_iterator( matrix_rows_iterator<U> const& src )
         : _ptr( src._ptr ),
           _row_size( src._row_size )
      {
      }

   protected:

      void
      increment()
      {
         _ptr += _row_size;
      }

      void
      decrement()
      {
         _ptr -= _row_size;
      }

      template< class U >
      bool
      equal( matrix_rows_iterator<U> const& op ) const
      {
         return _ptr == op._ptr;
      }

      reference_type
      dereference() const
      {
         return hpc::view<T>( _ptr, _row_size );
      }

      template< class U >
      size_t
      distance_to( view_iterator<U> const& op ) const
      {
         return ((size_t)(op._ptr - _ptr))/_row_size;
      }

      void
      advance( size_t dist )
      {
         _ptr += dist*_row_size;
      }

   protected:

      pointer_type _ptr;
      size_t _row_size;
   };

}

#endif
