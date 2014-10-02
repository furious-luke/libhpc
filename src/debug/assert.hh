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

#ifndef hpc_debug_assert_hh
#define hpc_debug_assert_hh

#include "libhpc/system/cuda.hh"

#if (!defined( NDEBUG ) || !defined( NEXCEPT ))

#ifdef CUDA_HOST

#include <sstream>
#include "libhpc/system/narg.hh"
#include "assertions.hh"

#ifndef NSTACKTRACE

#include "stacktrace.hh"

#define _ASSERT( expr, type, ... )                              \
   ((expr) ? (void)0 : (                                        \
      throw (type&)type().details(                              \
         #expr, __FILE__, __LINE__,                             \
         ::hpc::debug::stacktrace(),                            \
         ((const ::std::stringstream&)OSTREAM(                  \
            ::std::stringstream(), ##__VA_ARGS__ )).str()       \
         )))

#else // NSTACKTRACE

#define _ASSERT( expr, type, ... )                              \
   ((expr) ? (void)0 : (                                        \
      throw (type&)type().details(                              \
         #expr, __FILE__, __LINE__,                             \
         ((const ::std::stringstream&)OSTREAM(                  \
            ::std::stringstream(), ##__VA_ARGS__ )).str()       \
         )))

#endif // NSTACKTRACE

#else // __CUDA_ARCH__

#define _ASSERT( expr, ... )

#endif // __CUDAARCH__

#else // (!defined( NDEBUG ) || !defined( NEXCEPT ))

#define _ASSERT( expr, ... )

#endif // (!defined( NDEBUG ) || !defined( NEXCEPT ))

#ifndef NDEBUG

#define ASSERT( expr, ... )                                     \
   _ASSERT( expr, ::hpc::debug::assertion, ##__VA_ARGS__ )

#else

#define ASSERT( expr, ... )

#endif

#endif
