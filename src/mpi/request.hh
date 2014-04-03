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

#ifndef libhpc_mpi_request_hh
#define libhpc_mpi_request_hh

#include "init.hh"

namespace hpc {
   namespace mpi {

      class requests;

      class request
      {
         friend class requests;

      public:

	 request( MPI_Request req = MPI_REQUEST_NULL );

	 request( request const& src );

	 ~request();

	 void
	 mpi_request( MPI_Request req );

	 MPI_Request const&
	 mpi_request() const;

	 MPI_Request&
	 mod_mpi_request();

	 bool
	 test();

	 void
	 wait();

	 bool
	 operator==(const request& op) const;

	 bool
	 operator==(const MPI_Request op) const;

	 bool
	 operator!=(const request& op) const;

	 bool
	 operator!=(const MPI_Request op) const;

         operator bool() const;

      protected:

	 MPI_Request _req;
      };
   }
}

#endif
