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

#include "request.hh"
#include "insist.hh"

namespace hpc {
   namespace mpi {

      // void request::waitAll(VectorView<Request>& reqs) {
      //     mpi_insist(MPI_Waitall(reqs.getSize(), (MPI_Request*)(Request*)reqs, MPI_STATUSES_IGNORE), 10);
      // }

      request::request(MPI_Request req)
	 : _req(req)
      {
      }

      request::request(const request& req)
	 : _req(req._req)
      {
      }

      request::~request()
      {
         wait();
      }

      void
      request::mpi_request(MPI_Request req)
      {
	 this->wait();
	 this->_req = req;
      }

      const MPI_Request&
      request::mpi_request() const
      {
	 return this->_req;
      }

      MPI_Request&
      request::mod_mpi_request()
      {
	 this->wait();
	 return this->_req;
      }

      bool
      request::test()
      {
	 if( this->_req != MPI_REQUEST_NULL )
         {
	    int flag;
	    MPI_INSIST(MPI_Test((MPI_Request*)&this->_req, &flag, MPI_STATUS_IGNORE));
            this->_req = MPI_REQUEST_NULL;
	    return flag;
	 }
	 return true;
      }

      void
      request::wait() {
	 if(this->_req != MPI_REQUEST_NULL) {
	    MPI_INSIST(MPI_Wait(&this->_req, MPI_STATUS_IGNORE));
	    this->_req = MPI_REQUEST_NULL;
	 }
      }

      bool
      request::operator==(const request& op) const
      {
	 return this->_req == op._req;
      }

      bool
      request::operator==(const MPI_Request op) const
      {
	 return this->_req == op;
      }

      bool
      request::operator!=(const request& op) const
      {
	 return this->_req != op._req;
      }

      bool
      request::operator!=(const MPI_Request op) const
      {
	 return this->_req != op;
      }

      request::operator bool() const
      {
         return _req != MPI_REQUEST_NULL;
      }

   }
}
