#ifndef mpi_insist_hh
#define mpi_insist_hh

#include "libhpc/debug/debug.hh"

#define MPI_INSIST(call, ...)			\
  INSIST(call, == MPI_SUCCESS, ##__VA_ARGS__)

#endif
