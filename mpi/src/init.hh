#ifndef hpc_mpi_init_hh
#define hpc_mpi_init_hh

// #ifndef MPICH_SKIP_MPICXX
// #define MPICH_SKIP_MPICXX
// #endif
// #ifndef OMPI_SKIP_MPICXX
// #define OMPI_SKIP_MPICXX
// #endif
#include <mpi.h>
#include "logger.hh"

namespace hpc {
   namespace mpi {

#ifndef NLOG
      extern logger log;
#endif

      void initialise();

      void initialise(int& argc,
         char**& argv);

      void finalise(bool mpi=true);
   }
}

#endif
