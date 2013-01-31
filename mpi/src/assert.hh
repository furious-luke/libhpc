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

#ifndef mpi_assert_hh
#define mpi_assert_hh

#ifndef NDEBUG

#include "libhpc/debug/assert.hh"
#include "libhpc/debug/assertions.hh"
#include "comm.hh"

#ifndef NSTACKTRACE

#include "libhpc/debug/stacktrace.hh"

#define MPI_ASSERT( expr, comm, ... )           \
   ::hpc::mpi::_assert(                         \
      expr, comm, __FILE__, __LINE__, #expr,    \
      ::hpc::debug::stacktrace(), ##__VA_ARGS__ \
      )

#else

#define MPI_ASSERT( expr, comm, ... )                           \
   ::hpc::debug::_assert(                                       \
      expr, comm __FILE__, __LINE__, #expr, ##__VA_ARGS__       \
      )

#endif

// #define MPI_ASSERT( expr, comm, ... )           \
//    hpc::mpi::_assert(                           \
//       expr, comm, __FILE__, __LINE__, #expr,    \
//       ##__VA_ARGS__ )

namespace hpc {
   namespace mpi {

      void
      _assert( bool state,
	       const mpi::comm& comm,
	       const char* file,
	       int line,
	       const char* expr,
#ifndef NSTACKTRACE
               const debug::stacktrace& st,
#endif
	       const char* msg = NULL );

      void
      _assert( bool state,
               const mpi::comm& comm,
               const char* file,
               int line,
               const char* expr,
#ifndef NSTACKTRACE
               const debug::stacktrace& st,
#endif
               debug::assertion ass );
   }
}

#else

#define MPI_ASSERT( expr, comm, ... )

#endif

#endif
