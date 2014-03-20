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

#if (!defined( NDEBUG ) || !defined( NEXCEPT )) && !defined( __CUDACC__ )

#include <sstream>
#include "libhpc/system/narg.hh"
#include "assertions.hh"
#include "stacktrace.hh"

#ifndef NSTACKTRACE

#define _ASSERT( expr, type, ... )                              \
   ((expr) ? (void)0 : (                                        \
      throw (type&)type().details(                              \
         #expr, __FILE__, __LINE__,                             \
         ::hpc::debug::stacktrace(),                            \
         ((const ::std::stringstream&)OSTREAM(                  \
            ::std::stringstream(), ##__VA_ARGS__ )).str()       \
         )))

#else

#define _ASSERT( expr, type, ... )                              \
   ((expr) ? (void)0 : (                                        \
      throw (type&)type().details(                              \
         #expr, __FILE__, __LINE__,                             \
         ((const ::std::stringstream&)OSTREAM(                  \
            ::std::stringstream(), ##__VA_ARGS__ )).str()       \
         )))

#endif

#else

#define _ASSERT( expr, ... )

#endif

#ifndef NDEBUG

#define ASSERT( expr, ... )                                     \
   _ASSERT( expr, ::hpc::debug::assertion, ##__VA_ARGS__ )

#else

#define ASSERT( expr, ... )

#endif

#endif
