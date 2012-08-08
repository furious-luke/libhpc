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
