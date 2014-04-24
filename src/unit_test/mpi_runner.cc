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

#include <libhpc/mpi/comm.hh>
#include "mpi_runner.hh"
#include "failures.hh"

namespace hpc {
   namespace test {

      extern test_case_node_t* head;

      void
      mpi_runner::run( test_case_base& tc )
      {
         if( mpi::comm::world.rank() == 0 )
         {
            std::cout << tc.name();
            if( mpi::comm::world.size() > 1 )
               std::cout << "[" << mpi::comm::world.size() << "]";
            std::cout << " " << std::flush;
         }
         try
         {
            tc.run();
            if( mpi::comm::world.rank() == 0 )
               std::cout << " ok\n";
         }
         catch( test_failed& ex )
         {
            if( mpi::comm::world.rank() == 0 )
               std::cout << ex.what();
         }
      }

      void
      mpi_runner::dot() const
      {
         if( mpi::comm::world.rank() == 0 )
            runner::dot();
      }

   }
}
