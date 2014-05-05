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

   template< class T >
   class matrix
   {
   public:

      typedef typename std::vector<T>::size_type size_type;
      typedef          T                         value_type;

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

   protected:

      std::vector<T> _vec;
      std::array<size_type,2> _size;
   };

}

#endif
