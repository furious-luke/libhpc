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

#include "libhpc/debug/assertions.hh"

namespace hpc {

   extern hpc::application* global_app;

}

///
/// Entry point.
///
int
main( int argc,
      char* argv[] )
{
   typedef HPC_APP_CLASS application_type;
   int ec = 0;
   try
   {
      application_type app( argc, argv );
      hpc::global_app = &app;
      app();
   }
   catch( hpc::silent_terminate& ex )
   {
   }
#ifdef NDEBUG
   catch( hpc::exception& ex )
   {
      std::cerr << "\nERROR: " << ex.what() << "\n\n";
      ++ec;
   }
#endif
   return ec;
}

#endif
