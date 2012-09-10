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

#ifndef libhpc_numerics_simpson_hh
#define libhpc_numerics_simpson_hh

namespace hpc {
   namespace numerics {

      template< class Function >
      typename Function::value_type
      simpson( Function func,
               typename Function::value_type lower,
               typename Function::value_type upper,
               unsigned points )
      {
         typedef typename Function::value_type value_type;

         ASSERT( upper > lower );
         ASSERT( points > 1 );

         value_type width = upper - lower;
         value_type dx = width/(value_type)(points - 1);

         value_type part = 0.0;
         for( unsigned ii = 1; ii < points - 1; ii += 2 )
            part += func( lower + ((value_type)ii)*dx );
         value_type sum = 4.0*part;

         part = 0.0;
         for( unsigned ii = 2; ii < points - 1; ii +=2 )
            part += func( lower + ((value_type)ii)*dx );
         sum += 2.0*part;

         sum += func( lower ) + func( upper );
         sum *= dx/3.0;

         return sum;
      }
   }
}

#endif
