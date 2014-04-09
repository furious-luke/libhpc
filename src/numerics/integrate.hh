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

#ifndef libhpc_numerics_integrate_hh
#define libhpc_numerics_integrate_hh

namespace hpc {
   namespace num {

      template< class Function,
                class PointIter,
                class WeightIter >
      typename Function::result_type
      quadrature_summation( PointIter point_start,
                            const PointIter& point_finish,
                            WeightIter weight_start,
                            Function func )
      {
         typename Function::result_type sum = 0;
         while( point_start != point_finish )
            sum += (*weight_start++)*func( *point_start++ );
         return sum;
      }

   }
}

#endif
