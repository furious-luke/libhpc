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

#ifndef containers_new_hh
#define containers_new_hh

#include <stdlib.h>
#include <string.h>
#include <set>

#ifndef WITH_TAU

void* operator new(size_t size);
void operator delete(void* ptr);
void* operator new[](size_t size);
void operator delete[](void* ptr);

#endif

namespace hpc {
   namespace memory {

      void*
      new_alloc( size_t size );

      void
      new_free( void* ptr );

#if !defined NDEBUG && !defined WITH_TAU

      void check_ptr_cookie(void* ptr);

#endif

   }
}

#endif
