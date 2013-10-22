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

#include "application.hh"
#include "init.hh"
#include "comm.hh"

namespace hpc {
   namespace mpi {

      application::application( int argc,
                                char* argv[] )
         : hpc::application( argc, argv )
      {
         _rank = mpi::comm::world.rank();
         _size = mpi::comm::world.size();
      }

      application::~application()
      {
      }

      int
      application::rank() const
      {
         return _rank;
      }

      int
      application::size() const
      {
         return _size;
      }

   }
}
