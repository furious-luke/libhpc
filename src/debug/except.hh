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

#ifndef hpc_debug_except_hh
#define hpc_debug_except_hh

#include "libhpc/system/cuda.hh"

#if !defined(NEXCEPT) && defined(CUDA_HOST)

#include "assert.hh"

#define EXCEPT( expr, ... )                             \
  _ASSERT( expr, ::hpc::exception, ##__VA_ARGS__ )

#define EXCEPTAS( expr, type, ... )             \
   _ASSERT( expr, type, ##__VA_ARGS__ )

#else

#define EXCEPT( expr, ... )
#define EXCEPTAS( expr, type, ... )

#endif

#endif
