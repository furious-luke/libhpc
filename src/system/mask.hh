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

#ifndef libhpc_system_mask_hh
#define libhpc_system_mask_hh

namespace hpc {

   template< class T >
   T
   bit_mask( T size )
   {
      T mask = 0;
      if( size > 0 )
      {
         unsigned num_bits = log2i( size );
         for( unsigned ii = 0; ii < num_bits; ++ii )
            mask |= (1 << ii);
      }
      return mask;
   }

}

#endif
