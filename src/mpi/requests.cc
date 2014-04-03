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

#include "requests.hh"
#include "insist.hh"

namespace hpc {
   namespace mpi {

      requests::requests( size_t size )
	 : _reqs( size )
      {
         boost::fill( _reqs, MPI_REQUEST_NULL );
      }

      requests::~requests()
      {
	 wait_all();
      }

      void
      requests::clear()
      {
	 _reqs.clear();
      }

      bool
      requests::empty() const
      {
	 _reqs.empty();
      }

      void
      requests::reserve( size_t size )
      {
	 _reqs.reserve(size);
      }

      void
      requests::resize( size_t size )
      {
	 _reqs.resize(size);
      }

      vector<request>::size_type
      requests::size() const
      {
	 _reqs.size();
      }

      request&
      requests::append()
      {
	 _reqs.resize(_reqs.size() + 1);
	 return _reqs.back();
      }

      void
      requests::wait_all()
      {
	 if(_reqs.size())
	    MPI_INSIST(MPI_Waitall(_reqs.size(), (MPI_Request*)_reqs.data(), MPI_STATUSES_IGNORE));
      }

      bool
      requests::test_all()
      {
	 if(_reqs.size()) {
            int flag;
	    MPI_INSIST( MPI_Testall( _reqs.size(), (MPI_Request*)_reqs.data(), &flag, MPI_STATUSES_IGNORE ) );
            return flag;
         }
         return true;

	 // for(int ii = 0; ii < _reqs.size(); ++ii) {
	 //    int flag;

         //    // Note that we cannot use mod_mpi_request here, because it requires that
         //    // the communication be complete.
	 //    MPI_INSIST(MPI_Test(&_reqs[ii]._req, &flag, MPI_STATUS_IGNORE));
	 //    if(flag) {
	 //       _reqs.erase(_reqs.begin() + ii);
	 //       --ii;
	 //    }
	 // }
      }

      request&
      requests::operator[](int idx)
      {
	 return _reqs[idx];
      }

   }
}
