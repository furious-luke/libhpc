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

#ifndef libhpc_memory_new_hh
#define libhpc_memory_new_hh

#include <stdlib.h>

#ifndef NMEMSTATS
#define MEM_ENABLE_STATS()			\
   ::hpc::memory::enable_stats()
#else
#define MEM_ENABLE_STATS()
#endif

// NMEMDEBUG turns off all memory debugging routines, returning
// to fundamental "new" and "delete".
#ifndef NMEMDEBUG

void*
operator new( size_t size );

void
operator delete(void* ptr) noexcept( true );

void*
operator new[]( size_t size );

void
operator delete[]( void* ptr ) noexcept( true );

namespace hpc {
   namespace memory {

      void*
      new_alloc( size_t size );

      void
      new_free( void* ptr );

#if !defined NDEBUG && !defined WITH_TAU

      void
      check_ptr_cookie( void* ptr );

#endif

#ifndef NMEMSTATS
      void
      enable_stats( bool state=true );
#endif

   }
}

#endif

#endif
