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

#include "farmer2.hh"

namespace hpc {
   namespace algorithm {

      farm_object::farm_object( const mpi::comm& comm )
	 : _comm( &comm )
      {
      }

      master::master( const mpi::comm& comm )
	 : farm_object( comm )
      {
      }

      worker::worker( const mpi::comm& comm,
		      const mpi::comm& work_comm )
	 : farm_object( comm ),
	   _work_comm( &work_comm )
      {
      }

      void
      worker::set_work_comm( const mpi::comm& comm )
      {
	 _work_comm = &comm;
      }

   }
}
