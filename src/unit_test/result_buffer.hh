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

#ifndef hpc_unit_test_result_buffer_hh
#define hpc_unit_test_result_buffer_hh

#include <string.h>
#include "libhpc/system/cuda.hh"

namespace hpc {
   namespace test {

      template< class, class >
      class expression;

      template< class T = char >
      class result_buffer
      {
      public:

         typedef T char_type;

      public:

         CUDA_DEV_HOST
         result_buffer( char_type* buf = 0 )
         {
            set_buffer( buf );
         }

         CUDA_DEV_HOST
         void
         set_buffer( char_type* buf )
         {
            _buf = buf;
            _pos = 0;
            _first = true;
         }

         CUDA_DEV_HOST
         char_type*
         buffer()
         {
            return _buf;
         }

         template< class U,
                   class V >
         CUDA_DEV_HOST
         void
         push( expression<U,V> const& expr )
         {
            _buf[_pos++] = expr.result() ? 1 : 2;
            _buf[_pos] = 0;
         }

         CUDA_DEV_HOST
         void
         push( bool succ )
         {
            _buf[_pos++] = succ ? 1 : 2;
            _buf[_pos] = 0;
         }

      protected:

         char_type* _buf;
         size_t _pos;
         bool _first;
      };

   }
}

#endif
