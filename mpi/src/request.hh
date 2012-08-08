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
	 test() const;

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

      private:
	 MPI_Request _req;

         friend class requests;
      };
   }
}

#endif
