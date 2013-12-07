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

#ifndef libhpc_mpi_unit_test_main_hh
#define libhpc_mpi_unit_test_main_hh

#include <stdlib.h>
#include "libhpc/debug/unit_test.hh"
#include "libhpc/mpi/init.hh"
#ifdef HPC_UNIT_TEST_LOG
#include "libhpc/mpi/logger.hh"
#endif

int
main( int argc,
      char* argv[] )
{
   using namespace hpc;
   mpi::initialise( argc, argv );
#ifdef HPC_UNIT_TEST_LOG
   LOG_PUSH( new mpi::logger( "hpc_unit_test.log" ) );
#endif
   {
      test::runner runner;
      runner.run_all();
   }
   mpi::finalise();
   return EXIT_SUCCESS;
}

#endif
