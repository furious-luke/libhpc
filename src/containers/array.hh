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

#ifndef libhpc_containers_array_hh
#define libhpc_containers_array_hh

#include "libhpc/system/cuda.hh"
#ifndef __CUDACC__
#include <array>
#endif

namespace hpc {

#ifndef __CUDACC__

   template< class T,
             std::size_t N >
   using array = ::std::array<T,N>;

#else

   template< class T,
             std::size_t N >
   class array
   {
   public:

      T elems[N];

      CUDA_DEVICE_HOST
      T&
      operator[]( std::size_t idx )
      {
	 return elems[idx];
      }

      CUDA_DEVICE_HOST
      T const&
      operator[]( std::size_t idx ) const
      {
	 return elems[idx];
      }
   };

#endif

   template< class T >
   CUDA_DEVICE_HOST
   array<T,2>
   make_array( T const& x,
	       T const& y )
   {
#ifndef __CUDACC__
      return array<T,2>{ x, y };
#else
      array<T,2> arr;
      arr[0] = x;
      arr[1] = y;
      return arr;
#endif
   }

   template< class T >
   CUDA_DEVICE_HOST
   array<T,3>
   make_array( T const& x,
	       T const& y,
	       T const& z )
   {
#ifndef __CUDACC__
      return array<T,3>{ x, y, z };
#else
      array<T,3> arr;
      arr[0] = x;
      arr[1] = y;
      arr[2] = z;
      return arr;
#endif
   }

}

#endif
