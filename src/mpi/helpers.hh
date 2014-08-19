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

#ifndef hpc_mpi_helpers_hh
#define hpc_mpi_helpers_hh

#include <string>
#include "libhpc/system/math.hh"
#include "libhpc/system/array.hh"
#include "comm.hh"

namespace hpc {
   namespace mpi {

      std::string
      rank_string( mpi::comm const& comm = mpi::comm::world,
		   unsigned width = 5 );

      template< class T >
      hpc::array<T,2>
      modulo( T size,
	      mpi::comm const& comm = mpi::comm::world )
      {
	 return hpc::modulo<T>( size, comm.rank(), comm.size() );
      }

   }
}

#endif
