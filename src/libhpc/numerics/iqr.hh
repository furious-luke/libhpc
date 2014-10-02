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

#ifndef libhpc_numerics_iqr_hh
#define libhpc_numerics_iqr_hh

namespace hpc {
   namespace numerics {

      template< class Iter >
      typename Iter::value_type
      interquartile_range( Iter start,
                           unsigned size )
      {
         std::vector<typename Iter::value_type> set( size );
         for( unsigned ii = 0; ii < size; ++start, ++ii )
            set[ii] = *start;
         std::sort( set.begin(), set.end() );
         unsigned q = size/4;
         return set[size - q] - set[q];
      }

   }
}

#endif
