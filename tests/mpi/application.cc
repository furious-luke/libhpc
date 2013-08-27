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

#include "libhpc/debug/unit_test_main.hh"
#include "libhpc/mpi/application.hh"
#include "libhpc/mpi/init.hh"

using namespace hpc;
using namespace hpc::test;

namespace mpi_application {

   ///
   ///
   ///
   test_case<> ANON(
      "/mpi/application",
      "",
      []()
      {
         int argc = 0;
         char** argv = new char*[1];
         argv[0] = strdup( "test" );

         // Wrap in block to make sure it is destroyed afterwards.
         {
            mpi::application app( argc, argv, false );
            TEST( mpi::initialised() == true, "Must have been initialised." );
            TEST( app.size() == 1, "Must have communicator size of 1." );
            TEST( app.rank() == 0, "Must be master rank." );
         }
         TEST( mpi::initialised() == true, "Must not have been finalised." );

         // Free memory.
         free( argv[0] );
         delete argv;
      }
      );

}
