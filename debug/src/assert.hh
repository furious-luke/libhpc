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

#ifndef libhpc_debug_assert_hh
#define libhpc_debug_assert_hh

#include "assertions.hh"
#include "stacktrace.hh"

#if !defined( NDEBUG ) || !defined( NEXCEPT )

#ifndef NSTACKTRACE

#define _ASSERT( expr, ... )                    \
   ::hpc::debug::_assert(                       \
      expr, __FILE__, __LINE__, #expr,          \
      ::hpc::debug::stacktrace(), ##__VA_ARGS__ \
      )

#else

#define _ASSERT( expr, ... )                            \
   ::hpc::debug::_assert(                               \
      expr, __FILE__, __LINE__, #expr, ##__VA_ARGS__    \
      )

#endif

#else

#define _ASSERT( expr, ... )

#endif

namespace hpc {
   namespace debug {

#if !defined( NDEBUG ) || !defined( NEXCEPT )

      void
      _assert( bool state,
               const char* file,
               int line,
               const char* expr,
#ifndef NSTACKTRACE
               const stacktrace& st,
#endif
               const char* msg = NULL );

      void
      _assert( bool state,
               const char* file,
               int line,
               const char* expr,
#ifndef NSTACKTRACE
               const stacktrace& st,
#endif
               assertion asrt );

#endif

   }
}

#ifndef NDEBUG

#define ASSERT _ASSERT

#else

#define ASSERT( expr, ... )

#endif

#endif
