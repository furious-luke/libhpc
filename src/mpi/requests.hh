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
