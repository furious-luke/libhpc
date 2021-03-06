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

#include "morton.hh"

namespace hpc {

//    template<>
//    uint32_t
//    dilate<2>( uint16_t t )
//    {
//       uint32_t r = t;
//       r = (r | (r << 8)) & 0x00FF00FF;
//       r = (r | (r << 4)) & 0x0F0F0F0F;
//       r = (r | (r << 2)) & 0x33333333;
//       r = (r | (r << 1)) & 0x55555555;
//       return r;
//    }

//    template<>
//    uint16_t
//    undilate<2>( uint32_t t )
//    {
//       t = (t*  3) & 0x66666666;
//       t = (t*  5) & 0x78787878;
//       t = (t* 17) & 0x7F807F80;
//       t = (t*257) & 0x7FFF8000;
//       return ((uint16_t)(t >> 15));
//    }

//    template<>
//    uint32_t
//    dilate<3>( uint16_t t )
//    {
//       uint32_t r = t;
//       r = (r*0x10001) & 0xFF0000FF;
//       r = (r*0x00101) & 0x0F00F00F;
//       r = (r*0x00011) & 0xC30C30C3;
//       r = (r*0x00005) & 0x49249249;
//       return r;
//    }

//    template<>
//    uint16_t
//    undilate<3>( uint32_t t )
//    {
//       t = (t*0x00015) & 0x0E070381;
//       t = (t*0x01041) & 0x0FF80001;
//       t = (t*0x40001) & 0x0FFC0000;
//       return ((uint16_t)(t >> 18));
//    }

// #ifndef CXX_0X

//    uint32_t
//    morton( uint16_t x,
//            uint16_t y )
//    {
//       return (dilate<2>( y ) << 1) + dilate<2>( x );
//    }

//    uint32_t
//    morton( uint16_t x,
//            uint16_t y,
//            uint16_t z )
//    {
//       return (dilate<2>( z ) << 2) + morton( x, y );
//    }

// #endif

   uint32_t
   morton_array( boost::array<uint16_t,2> const& crd )
   {
#ifdef CXX_0X
      return morton_impl<2,0,uint16_t,uint16_t>::eval( crd[0], crd[1] );
#else
      return morton<2>( crd[0], crd[1] );
#endif
   }

   uint32_t
   morton_array( boost::array<uint16_t,3> const& crd )
   {
#ifdef CXX_0X
      return morton_impl<3,0,uint16_t,uint16_t,uint16_t>::eval( crd[0], crd[1], crd[2] );
#else
      return morton<3>( crd[0], crd[1], crd[2] );
#endif
   }

   // template<>
   // boost::array<uint16_t,2>
   // unmorton<2>( uint32_t idx )
   // {
   //    return boost::array<uint16_t,2>{
   //       undilate<2>(  idx & 0b01010101010101010101010101010101       ),
   //       undilate<2>( (idx & 0b10101010101010101010101010101010) >> 1 )
   //       };
   // }

   // template<>
   // boost::array<uint16_t,3>
   // unmorton<3>( uint32_t idx )
   // {
   //    return boost::array<uint16_t,3>{
   //       undilate<3>(  idx & 0b01001001001001001001001001001001       ),
   //       undilate<3>( (idx & 0b10010010010010010010010010010010) >> 1 ),
   //       undilate<3>( (idx & 0b00100100100100100100100100100100) >> 2 )
   //       };
   // }

}
