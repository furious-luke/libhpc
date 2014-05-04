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

#ifndef libhpc_algorithm_morton_hh
#define libhpc_algorithm_morton_hh

#include <stdint.h>
#include <boost/array.hpp>
#include "libhpc/system/cc_version.hh"

namespace hpc {

   template< int D >
   uint32_t
   dilate( uint16_t x );

   template< int D >
   uint16_t
   undilate( uint32_t );

   template<>
   uint32_t
   dilate<2>( uint16_t t );

   template<>
   uint16_t
   undilate<2>( uint32_t t );

   template<>
   uint32_t
   dilate<3>( uint16_t t );

   template<>
   uint16_t
   undilate<3>( uint32_t t );

#ifdef CXX_0X

   template< int D,
	     int N,
             class... Args >
   struct morton_impl;

   template< int D,
	     int N,
             class First,
             class... Args >
   struct morton_impl<D,N,First,Args...>
   {
      static
      uint32_t
      eval( uint32_t x,
            Args... args )
      {
	 return (dilate<D>( x ) << N) + morton_impl<D,N + 1,Args...>::eval( args... );
      }
   };

   template< int D,
	     int N >
   struct morton_impl<D,N>
   {
      static
      uint32_t
      eval()
      {
         return 0;
      }
   };

   template< int D,
             class... Args >
   uint32_t
   morton( Args... args )
   {
#ifndef __CUDACC__
      static_assert( sizeof...(args) == D, "Invalid number of Morton order parameters." );
#endif
      return morton_impl<D,0,Args...>::eval( args... );
   }

#else

   uint32_t
   morton( uint16_t x,
           uint16_t y );

   uint32_t
   morton( uint16_t x,
           uint16_t y,
           uint16_t z );

#endif

   uint32_t
   morton_array( boost::array<uint16_t,2> const& crd );

   uint32_t
   morton_array( boost::array<uint16_t,3> const& crd );

   template< int D >
   boost::array<uint16_t,D>
   unmorton( uint32_t idx );

   template<>
   boost::array<uint16_t,2>
   unmorton<2>( uint32_t idx );

   template<>
   boost::array<uint16_t,3>
   unmorton<3>( uint32_t idx );

}

#endif
