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
#include "libhpc/containers/array.hh"

namespace hpc {

   template< int D >
   uint32_t
   dilate( uint16_t x );

   template< int D >
   uint16_t
   undilate( uint32_t );

   template<>
   uint32_t
   dilate<2>( uint16_t t )
   {
      uint32_t r = t;
      r = (r | (r << 8)) & 0x00FF00FF;
      r = (r | (r << 4)) & 0x0F0F0F0F;
      r = (r | (r << 2)) & 0x33333333;
      r = (r | (r << 1)) & 0x55555555;
      return r;
   }

   template<>
   uint16_t
   undilate<2>( uint32_t t )
   {
      t = (t*  3) & 0x66666666;
      t = (t*  5) & 0x78787878;
      t = (t* 17) & 0x7F807F80;
      t = (t*257) & 0x7FFF8000;
      return ((uint16_t)(t >> 15));
   }

   template<>
   uint32_t
   dilate<3>( uint16_t t )
   {
      uint32_t r = t;
      r = (r*0x10001) & 0xFF0000FF;
      r = (r*0x00101) & 0x0F00F00F;
      r = (r*0x00011) & 0xC30C30C3;
      r = (r*0x00005) & 0x49249249;
      return r;
   }

   template<>
   uint16_t
   undilate<3>( uint32_t t )
   {
      t = (t*0x00015) & 0x0E070381;
      t = (t*0x01041) & 0x0FF80001;
      t = (t*0x40001) & 0x0FFC0000;
      return ((uint16_t)(t >> 18));
   }

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
      static_assert( sizeof...(args) == D, "Invalid number of Morton order parameters." );
      return morton_impl<D,0,Args...>::eval( args... );
   }

   uint32_t
   morton_array( hpc::array<uint16_t,2> const& crd )
   {
      return morton_impl<2,0,uint16_t,uint16_t>::eval( crd[0], crd[1] );
   }

   uint32_t
   morton_array( hpc::array<uint16_t,3> const& crd )
   {
      return morton_impl<3,0,uint16_t,uint16_t,uint16_t>::eval( crd[0], crd[1], crd[2] );
   }

   template< int D >
   hpc::array<uint16_t,D>
   unmorton( uint32_t idx );

   template<>
   hpc::array<uint16_t,2>
   unmorton<2>( uint32_t idx )
   {
      return hpc::array<uint16_t,2>{
         undilate<2>(  idx & 0b01010101010101010101010101010101       ),
         undilate<2>( (idx & 0b10101010101010101010101010101010) >> 1 )
         };
   }

   template<>
   hpc::array<uint16_t,3>
   unmorton<3>( uint32_t idx )
   {
      return hpc::array<uint16_t,3>{
         undilate<3>(  idx & 0b01001001001001001001001001001001       ),
         undilate<3>( (idx & 0b10010010010010010010010010010010) >> 1 ),
         undilate<3>( (idx & 0b00100100100100100100100100100100) >> 2 )
         };
   }

}

#endif
