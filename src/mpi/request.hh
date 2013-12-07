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

#ifndef mpi_request_hh
#define mpi_request_hh

#include "libhpc/containers/containers.hh"
#include "init.hh"

namespace hpc {
   namespace mpi {

      class requests;

      class request {
      public:

	 request(MPI_Request req=MPI_REQUEST_NULL);

	 request(const request& req);

	 ~request();

	 void
	 mpi_request(MPI_Request req);

	 const MPI_Request&
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

      private:
	 MPI_Request _req;

         friend class requests;
      };
   }
}

#endif
