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

#ifndef hpc_algorithm_morton_hh
#define hpc_algorithm_morton_hh

#include <stdint.h>
#include <boost/array.hpp>
#include <boost/utility/binary.hpp>
#include "libhpc/system/cc_version.hh"
#include "libhpc/system/cuda.hh"

namespace hpc {

   template< int D >
   CUDA_DEV_HOST
   uint32_t
   dilate( uint16_t x );

   template< int D >
   CUDA_DEV_HOST
   uint16_t
   undilate( uint32_t );

   template<>
   CUDA_DEV_HOST_INL
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
   CUDA_DEV_HOST_INL
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
   CUDA_DEV_HOST_INL
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
   CUDA_DEV_HOST_INL
   uint16_t
   undilate<3>( uint32_t t )
   {
      t = (t*0x00015) & 0x0E070381;
      t = (t*0x01041) & 0x0FF80001;
      t = (t*0x40001) & 0x0FFC0000;
      return ((uint16_t)(t >> 18));
   }

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
      static_assert( sizeof...(args) == D, "Invalid number of Morton order parameters." );
      return morton_impl<D,0,Args...>::eval( args... );
   }

#else // CXX_0X

   template< int D >
   CUDA_DEV_HOST_INL
   uint32_t
   morton( uint16_t x,
           uint16_t y )
   {
      return (dilate<D>( y ) << 1) + dilate<D>( x );
   }

   template< int D >
   CUDA_DEV_HOST_INL
   uint32_t
   morton( uint16_t x,
           uint16_t y,
           uint16_t z )
   {
      return (dilate<D>( z ) << 2) + morton<D>( x, y );
   }

#endif

   uint32_t
   morton_array( boost::array<uint16_t,2> const& crd );

   uint32_t
   morton_array( boost::array<uint16_t,3> const& crd );

   template< int D >
   boost::array<uint16_t,D>
   unmorton( uint32_t idx );

   template<>
   CUDA_DEV_HOST_INL
   boost::array<uint16_t,2>
   unmorton<2>( uint32_t idx )
   {
#ifdef CXX_0X
      return boost::array<uint16_t,2>{
         undilate<2>(  idx & BOOST_BINARY( 01010101 01010101 01010101 01010101 )       ),
         undilate<2>( (idx & BOOST_BINARY( 10101010 10101010 10101010 10101010 )) >> 1 )
         };
#else
      boost::array<uint16_t,2> res;
      res[0] = undilate<2>(  idx & BOOST_BINARY( 01010101 01010101 01010101 01010101 )       );
      res[1] = undilate<2>( (idx & BOOST_BINARY( 10101010 10101010 10101010 10101010 )) >> 1 );
      return res;
#endif
   }

   template<>
   CUDA_DEV_HOST_INL
   boost::array<uint16_t,3>
   unmorton<3>( uint32_t idx )
   {
#ifdef CXX_0X
      return boost::array<uint16_t,3>{
         undilate<3>(  idx & BOOST_BINARY( 01001001 00100100 10010010 01001001 )       ),
         undilate<3>( (idx & BOOST_BINARY( 10010010 01001001 00100100 10010010 )) >> 1 ),
         undilate<3>( (idx & BOOST_BINARY( 00100100 10010010 01001001 00100100 )) >> 2 )
         };
#else
      boost::array<uint16_t,3> res;
      res[0] = undilate<3>(  idx & BOOST_BINARY( 01001001 00100100 10010010 01001001 )       );
      res[1] = undilate<3>( (idx & BOOST_BINARY( 10010010 01001001 00100100 10010010 )) >> 1 );
      res[2] = undilate<3>( (idx & BOOST_BINARY( 00100100 10010010 01001001 00100100 )) >> 2 );
      return res;
#endif
   }

}

#endif
