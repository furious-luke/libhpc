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

#include "assert.hh"

#ifndef NDEBUG

namespace hpc {
   namespace mpi {

      extern bool use_abort;

      void
      _assert( bool state,
	       const mpi::comm& comm,
	       const char* file,
	       int line,
	       const char* expr,
	       const char* msg )
      {
	 _assert(state, comm, file, line, expr, debug::assertion(msg));
      }

      void
      _assert( bool state,
	       const mpi::comm& comm,
	       const char* file,
	       int line,
	       const char* expr,
	       debug::assertion ass )
      {
	 state = comm.all_reduce(state, MPI_LAND);
	 debug::_assert(state, file, line, expr, ass);
      }
   }
}

#endif
