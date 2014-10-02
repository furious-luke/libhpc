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

#ifndef hpc_system_array_hh
#define hpc_system_array_hh

#include <iostream>
#include "libhpc/system/cuda.hh"

namespace hpc {

   template< class T,
             std::size_t N >
   class array
   {
   public:

      typedef T        value_type;
      typedef T*       iterator;
      typedef T const* const_iterator;

   public:

      T elems[N];

      const_iterator
      begin() const
      {
	 return elems;
      }

      iterator
      begin()
      {
	 return elems;
      }

      const_iterator
      end() const
      {
	 return elems + N;
      }

      iterator
      end()
      {
	 return elems + N;
      }

      CUDA_DEV_HOST
      T&
      operator[]( std::size_t idx )
      {
	 return elems[idx];
      }

      CUDA_DEV_HOST
      T const&
      operator[]( std::size_t idx ) const
      {
	 return elems[idx];
      }
   };

   template< class T,
             size_t N >
   std::ostream&
   operator<<( std::ostream& strm,
               hpc::array<T,N> const& obj )
   {
      strm << "(";
      if( N )
      {
         typename hpc::array<T,N>::const_iterator it = obj.begin();
         strm << *it++;
         while( it != obj.end() )
            strm << ", " << *it++;
      }
      strm << ")";
      return strm;
   }

}

#endif
