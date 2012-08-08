#ifndef mpi_assert_hh
#define mpi_assert_hh

#ifndef NDEBUG

#include "libhpc/debug/assert.hh"
#include "libhpc/debug/assertions.hh"
#include "comm.hh"

#define MPI_ASSERT( expr, comm, ... )					\
   hpc::mpi::_assert(expr, comm, __FILE__, __LINE__, #expr, ##__VA_ARGS__)

namespace hpc {
   namespace mpi {

      void
      _assert( bool state,
	       const mpi::comm& comm,
	       const char* file,
	       int line,
	       const char* expr,
	       const char* msg=NULL );

      void _assert( bool state,
		    const mpi::comm& comm,
		    const char* file,
		    int line,
		    const char* expr,
		    debug::assertion ass );
   }
}

#else

#define MPI_ASSERT( expr, comm, ... )

#endif

#endif
