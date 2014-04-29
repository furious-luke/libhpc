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

namespace hpc {

   template< class T >
   class matrix
   {
   public:

      typedef typename std::vector<T>::size_type size_type;

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

   protected:

      std::vector<T> _vec;
      std::array<size_type,2> _size;
   };

}

#endif
