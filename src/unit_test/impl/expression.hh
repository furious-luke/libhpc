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

#ifndef hpc_unit_test_impl_expression_hh
#define hpc_unit_test_impl_expression_hh

#include <iostream>
#include <string>
#include "libhpc/system/cuda.hh"

namespace hpc {
   namespace test {

      template< class T,
                class U >
      CUDA_DEV_HOST
      void
      expression<T,U>::test( result_buffer<>& rb )
      {
         rb.push( *this );
      }

   }
}

#endif
