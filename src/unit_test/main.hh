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

#ifndef hpc_unit_test_main_hh
#define hpc_unit_test_main_hh

#include <stdlib.h>
#include "unit_test.hh"
#include "runner.hh"
#include "fixtures.hh"
#ifdef HPC_UT_LOG
#include <libhpc/logging.hh>
#endif
#ifdef HPC_UT_MPI
#include <libhpc/mpi.hh>
#include "mpi_runner.hh"
#endif

int
main( int argc,
      char* argv[] )
{
#ifdef HPC_UT_MPI
   hpc::mpi::initialise( argc, argv );
#endif
#ifdef HPC_UT_LOG
   LOG_PUSH( new hpc::log::stdout );
#endif
#ifdef HPC_UT_MPI
   hpc::test::mpi_runner runner;
#else
   hpc::test::runner runner;
#endif
   runner.run_all();
#ifdef HPC_UT_MPI
   hpc::mpi::finalise();
#endif
   return EXIT_SUCCESS;
}

#endif
