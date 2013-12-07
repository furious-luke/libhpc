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

#ifndef libhpc_main_main_hh
#define libhpc_main_main_hh

#ifndef HPC_APP_CLASS
#error libhpc: Must have an application class defined.
#endif

#include <libhpc/debug/assertions.hh>
#include <libhpc/mpi/init.hh>
#include <libhpc/mpi/comm.hh>

namespace hpc {

   extern hpc::application* global_app;

}

int
main( int argc,
      char* argv[] )
{
   typedef HPC_APP_CLASS application_type;
   {
      hpc::mpi::initialise( argc, argv );
      try
      {
	 application_type app( argc, argv );
         hpc::global_app = &app;
	 app();
      }
      catch( std::exception& ex )
      {
         std::cerr << "\nError: " << ex.what() << "\n\n";
	 hpc::mpi::comm::world.abort();
      }
      hpc::mpi::finalise();
   }
   return EXIT_SUCCESS;
}

#endif
