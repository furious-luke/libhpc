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

#ifndef NDEBUG

#include "assert.hh"

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
	       const char* msg )
      {
	 _assert(
            state, comm, file, line, expr,
#ifndef NSTACKTRACE
            st,
#endif
            debug::assertion( msg )
            );
      }

      void
      _assert( bool state,
	       const mpi::comm& comm,
	       const char* file,
	       int line,
	       const char* expr,
#ifndef NSTACKTRACE
               const debug::stacktrace& st,
#endif
	       debug::assertion asrt )
      {
	 state = comm.all_reduce( state, MPI_LAND );
	 debug::_assert(
            state, file, line, expr,
#ifndef NSTACKTRACE
            st,
#endif
            asrt
            );
      }
   }
}

#endif
