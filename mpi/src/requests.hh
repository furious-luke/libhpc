#ifndef mpi_requests_hh
#define mpi_requests_hh

#include "libhpc/containers/containers.hh"
#include "init.hh"
#include "request.hh"

namespace hpc {
   namespace mpi {

      class requests {
      public:

	 requests( index size = 0 );

	 ~requests();

	 void
	 clear();

	 bool
	 empty() const;

         void
         reserve( index size );

	 void
	 resize( index size );

         vector<request>::size_type
         size() const;

	 request&
	 append();

	 void
	 wait_all();

         bool
	 test_all();

	 request&
	 operator[](int idx);

      private:
	 vector<request> _reqs;
      };
   }
}

#endif
